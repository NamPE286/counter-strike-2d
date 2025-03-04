#include "Weapon.hpp"

#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"

Weapon::Weapon(SDL_Renderer* renderer, std::string name):
	MonoBehaviour(renderer), name(name)
{
	if (name == "Knife") {
		return;
	}
	
	if (name == "AK-47") {
		magSize = 30, mobility = 215, range = 8192, killAward = 1500;
		damage = 36.0f, armorPenetration = 0.775f, tagging = 0.6f,
		headshotMultiplier = 4.0f, standingInaccuracy = 7.1f, runningInaccuracy = 5.41f,
		fireRate = 100.0f, cooldown = -1.0f;
		automatic = true;
		ammo = 30, reserveAmmo = 90;
	} else if (name == "M4A4") {

	} else if (name == "Glock-18") {

	} else if (name == "USP-S") {

	} else {
		throw std::runtime_error("Weapon does not exist");
	}
}

void Weapon::fire(Vec2* position) {
	pos = position;
	firing = true;
}

void Weapon::stopFire() {
	firing = false;
}

void Weapon::update() {
	cooldown -= Time::deltaTime;

	if (cooldown <= 0.0f) {
		cooldown = -1.0f;
	}

	if (firing) { 
		int x = 0, y = 0;

		SDL_GetMouseState(&x, &y);

		float angle = Utils::getAngle((int)pos->x, (int)pos->y, x, y);

		if (automatic && cooldown <= 0.0f) {
			bullets.emplace_back(renderer, angle, (int)pos->x, (int)pos->y, range);
			cooldown = fireRate;
		}

		if (!automatic) {
			bullets.emplace_back(renderer, angle, (int)pos->x, (int)pos->y, range);
			firing = false;
		}
	}

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