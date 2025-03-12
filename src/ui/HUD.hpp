#pragma once

#include <SDL2/SDL.h>

#include "../ui/Text.hpp"
#include "../gameObjects/Player.hpp"
#include "../behaviours/MonoBehaviour.hpp"

class HUD : public MonoBehaviour {
	Player* player = nullptr;
	Text *ammoText = nullptr, *reserveAmmoText = nullptr, *moneyText = nullptr;
	SDL_Color color = { 0, 0, 0, 0 };

public:
	HUD(SDL_Renderer* renderer, Player* player, SDL_Color color);

	void update();
	void render();
};