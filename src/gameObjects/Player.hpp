#pragma once

#include <array>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <string>

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Bullet.hpp"
#include "../geometry/Vec2.hpp"
#include "Weapon.hpp"

class GameScene;

enum PlayerSide {
	T,
	CT,
	SPECTATOR
};

class Player : public MonoBehaviour {
	std::map<Uint32, Vec2> directionMap = {
		{ SDL_SCANCODE_W, Vec2(0, -1) },
		{ SDL_SCANCODE_A, Vec2(-1, 0) },
		{ SDL_SCANCODE_S, Vec2(0, 1) },
		{ SDL_SCANCODE_D, Vec2(1, 0) }
	};
	SDL_Color color = { 0, 0, 0, 0 };
	GameScene *target = nullptr;

	float maxSpeed = MAX_SPEED;
	float footstepDelay = 0;

	void update_position();
	void change_weapon(int slot);

public:
	const float MAX_SPEED = 0.15f;
	const float MAX_ACCELERATION = 0.0005f;
	bool keyboard[SDL_NUM_SCANCODES + 1] = {};

	const int size = 25, borderWidth = 6;

	Vec2 position, velocity, acceleration, direction, prevPosition;
	int hp = 100, armor = 0, money = 16000;
	int side = PlayerSide::T;
	int kill = 0, death = 0, assist = 0, damageInflicted = 0;
	bool playable = true, helmet = false;
	std::array<std::shared_ptr<Weapon>, 3> weapons;
	int weaponSlot = 0;
	std::string name;

	Player(SDL_Renderer *renderer, GameScene *scene, std::string name, int side = PlayerSide::T, Vec2 pos = Vec2(0, 0), bool playable = true);
	~Player();

	void update();
	void fixed_update();
	void render();
	void on_key_down(SDL_Event &event);
	void on_key_up(SDL_Event &event);
	void fire();
	void stop_firing();
	void set_position(Vec2 newPos);
	void take_damage(Weapon *w, bool headshot);
	void reset();
	void stop_movement();
	void stop_movement_x();
	void stop_movement_y();
	void switch_side();

	bool collide(Bullet bullet);
	
	float distance(Bullet bullet);

	Weapon *get_weapon() const;
};