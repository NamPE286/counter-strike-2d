#include "Weapon.hpp"

#include <iostream>
#include <thread>
#include <stdexcept>

#include "../managers/Time.hpp"
#include "../managers/Mouse.hpp"
#include "../utilities/Utils.hpp"
#include "../scenes/GameScene.hpp"

void Weapon::fixed_update_fire() {
	fireCooldown -= Time::fixedDeltaTime;
	fireCooldown = std::max(fireCooldown, -1.0f);

	if (!firing || fireCooldown > 0.0f) {
		return;
	}

	int x = Mouse::x, y = Mouse::y;
	float variance = 0.0f;

	if (*vel == Vec2(0, 0)) {
		variance = standingInaccuracy;
	} else {
		variance = runningInaccuracy;
	}

	x += (int)Utils::getRandom(variance);
	y += (int)Utils::getRandom(variance);

	float angle = Utils::getAngle((int)pos->x, (int)pos->y, x, y);

	if (automatic) {
		if (ammo > 0) {
			add_bullet((int)pos->x, (int)pos->y, angle, range);

			fireCooldown = fireRate;
			ammo--;
		} else if (ammo == -1) {
			add_bullet((int)pos->x, (int)pos->y, angle, range);

			fireCooldown = fireRate;
		}
	} else {
		if (ammo > 0) {
			add_bullet((int)pos->x, (int)pos->y, angle, range);

			fireCooldown = fireRate;
			firing = false;
			ammo--;
		}
	}

	if (ammo == 0) {
		firing = false;
		reload();

		return;
	}
}

void Weapon::fixed_update_reload() {
	if (!reloading || reserveAmmo <= 0) {
		reloading = false;
		return;
	}

	reloadCooldown -= Time::fixedDeltaTime;

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
		Mix_PlayChannel(-1, Audio::loadWAV("assets/weapons/lowammo_01.wav"), 0);
	}
}

void Weapon::play_draw_sound() {
	std::thread t([&]() {
		Mix_PlayChannel(-1, drawSound, 0);
		SDL_Delay(300);

		if (pullSound != nullptr) {
			Mix_PlayChannel(-1, pullSound, 0);
			SDL_Delay(500);
		}

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
			} else {
				SDL_Delay(400);
			}
		}
		});

	t.detach();
}

void Weapon::add_bullet(int x, int y, float angle, int range) {
	Bullet tmp(renderer, angle, x, y, range);
	tmp.length = target->map->distance(x, y, angle, range);

	bullets.emplace_back(std::make_shared<Bullet>(tmp));
	bulletQueue.push(tmp);

	play_firing_sound(ammo <= magSize * 20 / 100);
}

Weapon::Weapon(SDL_Renderer *renderer, std::string name, Vec2 *pos, Vec2 *vel):
	MonoBehaviour(renderer), name(name), pos(pos), vel(vel)
{
	if (name == "Knife") {
		drawSound = Audio::loadWAV("assets/weapons/knife/knife_deploy1.wav");
		firingSound = Audio::loadWAV("assets/weapons/knife/knife_slash1.wav");
	} else if (name == "AK-47") {
		magSize = 30, mobility = 215, range = 1000, killReward = 300, price = 2700;
		baseDamage = 36.0f, armorPenetration = 0.775f, taggingPower = 0.6f,
			headshotMultiplier = 4.0f, standingInaccuracy = 7.01f, runningInaccuracy = 182.07f,
			fireRate = 100.0f, reloadTime = 2400.0f;
		automatic = true;
		ammo = 30, reserveAmmo = reserveAmmoBase = 90;

		drawSound = Audio::loadWAV("assets/weapons/ak47/ak47_draw.wav");
		firingSound = Audio::loadWAV("assets/weapons/ak47/ak47_01.wav");
		pullSound = Audio::loadWAV("assets/weapons/ak47/ak47_boltpull.wav");
		reloadSound = {
			Audio::loadWAV("assets/weapons/ak47/ak47_clipin.wav"),
			Audio::loadWAV("assets/weapons/ak47/ak47_clipout.wav"),
			Audio::loadWAV("assets/weapons/ak47/ak47_boltpull.wav")
		};
	} else if (name == "M4A4") {

	} else if (name == "Glock-18") {
		magSize = 20, mobility = 240, range = 400, killReward = 300, price = 200;
		baseDamage = 30.0f, armorPenetration = 0.47f, taggingPower = 0.5f,
			headshotMultiplier = 4.0f, standingInaccuracy = 7.6f, runningInaccuracy = 17.6f,
			fireRate = 50.0f, reloadTime = 2300.0f;
		automatic = false;
		ammo = 20, reserveAmmo = reserveAmmoBase = 120;

		drawSound = Audio::loadWAV("assets/weapons/glock18/glock_draw.wav");
		firingSound = Audio::loadWAV("assets/weapons/glock18/glock_01.wav");
		pullSound = Audio::loadWAV("assets/weapons/glock18/glock_slideback.wav");
		reloadSound = {
			Audio::loadWAV("assets/weapons/glock18/glock_clipin.wav"),
			Audio::loadWAV("assets/weapons/glock18/glock_clipout.wav"),
			Audio::loadWAV("assets/weapons/glock18/glock_slideback.wav"),
			Audio::loadWAV("assets/weapons/glock18/glock_sliderelease.wav")
		};
	} else if (name == "USP-S") {

	} else {
		throw std::runtime_error("Weapon does not exist");
	}
}

void Weapon::equip(bool playSound) {
	pullingOut = true;

	if (playSound) {
		play_draw_sound();
	}
}

void Weapon::fire(GameScene *scene) {
	target = scene;

	if (reloading || pullingOut) {
		return;
	}

	firing = true;
}

void Weapon::stop_firing() {
	firing = false;
}

void Weapon::reload() {
	if (ammo == magSize) {
		std::cout << "Ammo at max" << '\n';
		return;
	}

	if (reloading || reserveAmmo == 0) {
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
	std::vector<std::shared_ptr<Bullet>> tmp;

	for (auto &i : bullets) {
		i->update();

		if (!i->finished()) {
			tmp.push_back(i);
		}
	}

	bullets.clear();

	for (auto &i : tmp) {
		bullets.push_back(i);
	}
}

void Weapon::fixed_update() {
	fixed_update_fire();
	fixed_update_reload();
}

void Weapon::render() {
	for (auto &i : bullets) {
		i->render();
	}
}

void Weapon::reset() {
	ammo = magSize;
	reserveAmmo = reserveAmmoBase;
}

bool Weapon::poll_bullets(Bullet &bullet) {
	if (bulletQueue.empty()) {
		return false;
	}

	bullet = bulletQueue.front();
	bulletQueue.pop();

	return true;
}