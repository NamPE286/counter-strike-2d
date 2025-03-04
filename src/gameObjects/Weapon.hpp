#include <string>
#include <queue>
#include <stdexcept>
#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "Bullet.hpp"

class Weapon : public MonoBehaviour {
	int magSize = -1, mobility = 250, range = 10, killAward = 1500, rpm = 150;
	float damage = 0.0f, armorPenetration = 0.85f, tagging = 0.0f, headshotMultiplier = 1.0f, standingInaccuracy = 0.0f, runningInaccuracy = 0.0f, elapsed = 0.0f;
	bool firing = false, automatic = true;
	std::queue<Bullet> bullets;
public:
	std::string name = "Knife";
	int ammo = -1, reserveAmmo = -1;
	int price = 0;

	Weapon(SDL_Renderer* renderer, std::string name);
	void fire(float angle, int x, int y);
	void update();
};