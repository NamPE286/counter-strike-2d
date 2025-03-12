#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "Bullet.hpp"
#include "../geometry/Vec2.hpp"
#include "../managers/Audio.hpp"

class Weapon : public MonoBehaviour {
	Vec2 *pos = nullptr, *vel = nullptr;
	int magSize = -1, range = 50;
	float damage = 25.0f, armorPenetration = 0.85f, tagging = 0.0f,
		headshotMultiplier = 1.0f, standingInaccuracy = 0.0f, runningInaccuracy = 0.0f,
		fireRate = 400.0f, fireCooldown = -1.0f, reloadTime = 0.0f, reloadCooldown = -1.0f;
	bool firing = false, reloading = false, pullingOut = true, automatic = true;
	std::vector<Bullet> bullets;
	Mix_Chunk *drawSound = nullptr, *firingSound = nullptr, *pullSound = nullptr;
	std::vector<Mix_Chunk*> reloadSound;

	void fixed_update_fire();
	void fixed_update_reload();
	void play_firing_sound(bool lowAmmo);
	void play_draw_sound();
	void play_reload_sound();

public:
	std::string name = "Knife";
	int ammo = -1, reserveAmmo = -1;
	int price = 0, mobility = 250, killReward = 1500;

	Weapon(SDL_Renderer* renderer, std::string name, Vec2* pos, Vec2* vel);

	void equip();
	void fire(Vec2* position, Vec2* velocity);
	void stop_firing();
	void reload();
	void stop_reloading();
	void update();
	void fixed_update();
	void render();
};