#include "Weapon.hpp"

#include <iostream>
#include <thread>

#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"

void Weapon::update_fire() {
	if (!firing) {
		return;
	}

	fireCooldown -= Time::deltaTime;

	if (fireCooldown <= 0.0f) {
		fireCooldown = -1.0f;
	}

	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);

	float angle = Utils::getAngle((int)pos->x, (int)pos->y, x, y);

	if (automatic && fireCooldown <= 0.0f) {
		bullets.emplace_back(renderer, angle, (int)pos->x, (int)pos->y, range);
		fireCooldown = fireRate;

		if (ammo > 0) {
			ammo--;
			play_firing_sound(ammo <= magSize * 20 / 100);
			std::cout << ammo << ' ' << reserveAmmo << '\n';
		}
	}

	if (!automatic) {
		bullets.emplace_back(renderer, angle, (int)pos->x, (int)pos->y, range);
		firing = false;

		if (ammo > 0) {
			ammo--;
			play_firing_sound(ammo <= magSize * 20 / 100);
			std::cout << ammo << ' ' << reserveAmmo << '\n';
		}
	}

	if (ammo == 0) {
		firing = false;
		reload();

		return;
	}
}

void Weapon::update_reload() {
	if (!reloading || reserveAmmo <= 0) {
		reloading = false;
		return;
	}

	reloadCooldown -= Time::deltaTime;

	if (reloadCooldown > 0.0f) {
		return;
	}

	reloadCooldown = -1.0f;
	reloading = false;

	int r = std::min(magSize - ammo, reserveAmmo);

	ammo += r;
	reserveAmmo -= r;

	std::cout << "Reloaded" << '\n';
}

void Weapon::play_firing_sound(bool lowAmmo) {
	Mix_HaltChannel(1);
	Mix_PlayChannel(1, firingSound, 0);

	if (lowAmmo) {
		Mix_PlayChannel(-1, lowAmmoSound, 0);
	}
}

void Weapon::play_draw_sound() {
	std::thread t([&]() {
		Mix_PlayChannel(-1, drawSound, 0);
		SDL_Delay(300);
		Mix_PlayChannel(-1, pullSound, 0);
		SDL_Delay(500);
		pullingOut = false;
	});

	t.detach();
}

void Weapon::play_reload_sound() {
	std::thread t([&]() {
		SDL_Delay(200);

		for (int i = 0; i < (int)reloadSound.size(); i++) {
			int channel = Mix_PlayChannel(-1, reloadSound[i], 0);

			while (Mix_Playing(channel) != 0) {
				SDL_Delay(0);
			}

			if (i == 0) {
				SDL_Delay(int(reloadTime / 3));
			} else if (i == 2) {
				SDL_Delay(10);
			}
			else {
				SDL_Delay(400);
			}
		}
	});

	t.detach();
}

Weapon::Weapon(SDL_Renderer* renderer, std::string name):
	MonoBehaviour(renderer), name(name)
{
	if (name == "Knife") {
		drawSound = Mix_LoadWAV("assets/weapons/knife/knife_deploy1.wav");
		firingSound = Mix_LoadWAV("assets/weapons/knife/knife_slash1.wav");
	} else if (name == "AK-47") {
		magSize = 30, mobility = 215, range = 1000, killReward = 300, price = 2700;
		damage = 36.0f, armorPenetration = 0.775f, tagging = 0.6f,
		headshotMultiplier = 4.0f, standingInaccuracy = 7.8f, runningInaccuracy = 5.41f,
		fireRate = 100.0f, reloadTime = 2400.0f;
		automatic = true;
		ammo = 30, reserveAmmo = 90;

		drawSound = Mix_LoadWAV("assets/weapons/ak47/ak47_draw.wav");
		firingSound = Mix_LoadWAV("assets/weapons/ak47/ak47_01.wav");
		pullSound = Mix_LoadWAV("assets/weapons/ak47/ak47_boltpull.wav");
		reloadSound = {
			Mix_LoadWAV("assets/weapons/ak47/ak47_clipin.wav"),
			Mix_LoadWAV("assets/weapons/ak47/ak47_clipout.wav"),
			Mix_LoadWAV("assets/weapons/ak47/ak47_boltpull.wav")
		};
	} else if (name == "M4A4") {

	} else if (name == "Glock-18") {
		magSize = 20, mobility = 240, range = 400, killReward = 300, price = 200;
		damage = 30.0f, armorPenetration = 0.47f, tagging = 0.5f,
		headshotMultiplier = 4.0f, standingInaccuracy = 56.0f, runningInaccuracy = 17.6f,
		fireRate = 50.0f, reloadTime = 2300.0f;
		automatic = false;
		ammo = 20, reserveAmmo = 120;

		drawSound = Mix_LoadWAV("assets/weapons/glock18/glock_draw.wav");
		firingSound = Mix_LoadWAV("assets/weapons/glock18/glock_01.wav");
		pullSound = Mix_LoadWAV("assets/weapons/glock18/glock_slideback.wav");
		reloadSound = {
			Mix_LoadWAV("assets/weapons/glock18/glock_clipin.wav"),
			Mix_LoadWAV("assets/weapons/glock18/glock_clipout.wav"),
			Mix_LoadWAV("assets/weapons/glock18/glock_slideback.wav"),
			Mix_LoadWAV("assets/weapons/glock18/glock_sliderelease.wav")
		};
	} else if (name == "USP-S") {

	} else {
		throw std::runtime_error("Weapon does not exist");
	}
}

Weapon::~Weapon() {
	Mix_FreeChunk(drawSound);
	Mix_FreeChunk(firingSound);
	Mix_FreeChunk(pullSound);

	for (auto i : reloadSound) {
		Mix_FreeChunk(i);
	}
}

void Weapon::equip() {
	pullingOut = true;
	play_draw_sound();
}

void Weapon::fire(Vec2* position) {
	if (reloading || pullingOut) {
		return;
	}

	pos = position;
	firing = true;
}

void Weapon::stop_firing() {
	firing = false;
	fireCooldown = -1.0f;
}

void Weapon::reload() {
	if (ammo == magSize) {
		std::cout << "Ammo at max" << '\n';
		return;
	}

	if (reloading) {
		return;
	}

	std::cout << "Reloading" << '\n';

	reloading = true;
	reloadCooldown = reloadTime;
	play_reload_sound();
}

void Weapon::stop_reloading() {
	reloading = false;
	reloadCooldown = -1.0f;
}

void Weapon::update() {
	update_fire();
	update_reload();

	std::vector<Bullet> tmp;

	for (auto& i : bullets) {
		i.update();

		if (!i.finished()) {
			tmp.push_back(i);
		}
	}

	bullets.clear();

	for (auto& i : tmp) {
		bullets.push_back(i);
	}
}

void Weapon::render() {
	for (auto& i : bullets) {
		i.render();
	}
}