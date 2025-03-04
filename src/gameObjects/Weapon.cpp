#include "Weapon.hpp"

Weapon::Weapon(SDL_Renderer* renderer, std::string name):
	MonoBehaviour(renderer), name(name)
{
	if (name == "Knife") {
		return;
	}
	
	if (name == "AK-47") {

	} else if (name == "M4A4") {

	} else if (name == "Glock-18") {

	} else if (name == "USP-S") {

	} else {
		throw std::runtime_error("Weapon does not exist");
	}
}

void Weapon::fire(float angle, int x, int y) {
	bullets.emplace(renderer, angle, x, y);
}

void Weapon::update() {
	while (bullets.front().finished()) {
		bullets.pop();
	}
}
