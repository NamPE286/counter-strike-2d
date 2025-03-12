#pragma once

#include <SDL2/SDL.h>
#include <utility>
#include <map>
#include <string>

#include "../behaviours/MonoBehaviour.hpp"
#include "../geometry/Vec2.hpp"
#include "Weapon.hpp"

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
	Vec2 position, velocity, acceleration, direction;
	Weapon* weapons[3] = { nullptr, nullptr, nullptr };
	SDL_Color color = { 0, 0, 0, 0 };
	
	const int size = 30, borderWidth = 8;
	float maxSpeed = 0.4f;
	int weaponSlot = 0;
	float footstepDelay = 0;

	void update_position();
public:
	int hp = 100, armor = 0, money = 800;
	int side = PlayerSide::T;
	int kill = 0, death = 0, assist = 0;
	bool playable = true;
	std::string name;

	Player(SDL_Renderer* renderer, std::string name, int side = PlayerSide::T, Vec2 pos = Vec2(0, 0), bool playable = true);
	~Player();

	void update();
	void fixed_update();
	void render();
	void on_key_down(SDL_Event& event);
	void on_key_up(SDL_Event& event);
	void on_mouse_button_down(SDL_Event& event);
	void on_mouse_button_up(SDL_Event& event);
	Weapon* get_weapon();
};