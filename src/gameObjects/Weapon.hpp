#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <thread>
#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "Bullet.hpp"
#include "../geometry/Vec2.hpp"

class Weapon : public MonoBehaviour {
	Vec2* pos = nullptr;
	int magSize = -1, mobility = 250, range = 50, killReward = 1500;
	float damage = 25.0f, armorPenetration = 0.85f, tagging = 0.0f,
		headshotMultiplier = 1.0f, standingInaccuracy = 0.0f, runningInaccuracy = 0.0f,
		fireRate = 400.0f, fireCooldown = -1.0f, reloadTime = 0.0f, reloadCooldown = -1.0f;
	bool firing = false, reloading = false, automatic = true;
	std::vector<Bullet> bullets;

	void update_fire();
	void update_reload();
public:
	std::string name = "Knife";
	int ammo = -1, reserveAmmo = -1;
	int price = 0;

	Weapon(SDL_Renderer* renderer, std::string name);
	void fire(Vec2* position);
	void stop_firing();
	void reload();
	void stop_reloading();
	void update();
	void render();
};