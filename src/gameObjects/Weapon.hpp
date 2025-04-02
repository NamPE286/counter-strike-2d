#pragma once

#include <memory>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

#include "../behaviours/MonoBehaviour.hpp"
#include "../geometry/Vec2.hpp"
#include "Bullet.hpp"

class GameScene;
class Player;

class Weapon : public MonoBehaviour {
	int magSize = -1;
	int reserveAmmoBase = -1;
	float standingInaccuracy = 0.0f, runningInaccuracy = 0.0f,
		fireRate = 400.0f, fireCooldown = -1.0f, reloadTime = 0.0f, reloadCooldown = -1.0f;
	bool firing = false, reloading = false, pullingOut = true, automatic = true;
	Mix_Chunk *drawSound = nullptr, *firingSound = nullptr, *pullSound = nullptr;
	std::vector<Mix_Chunk *> reloadSound;
	std::vector<std::shared_ptr<Bullet>> bullets;
	std::queue<Bullet> bulletQueue;
	GameScene *target = nullptr;
	Vec2 *fireTarget = nullptr;

	void fixed_update_fire();
	void fixed_update_reload();
	void play_firing_sound(bool lowAmmo);
	void play_draw_sound();
	void play_reload_sound();
	void add_bullet(int x, int y, float angle, int range);
public:
	Player *owner = nullptr;
	std::string name = "Knife";
	float baseDamage = 25.0f, armorPenetration = 0.85f, taggingPower = 0.0f, headshotMultiplier = 1.0f, range = 50;
	int ammo = -1, reserveAmmo = -1;
	int price = 0, mobility = 250, killReward = 1500;
	char symbol = '-';

	Weapon(SDL_Renderer *renderer, std::string name, Player *owner, GameScene *scene);

	void equip(bool playSound);
	void fire();
	void stop_firing();
	void reload();
	void stop_reloading();
	void update();
	void fixed_update();
	void render();
	void reset();

	// If NULL is passed then default to mouse position
	void set_fire_target(Vec2 *t);

	bool poll_bullets(Bullet &bullet);
};