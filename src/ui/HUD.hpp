#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "../ui/Text.hpp"
#include "../gameObjects/Player.hpp"
#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"
#include "../gameObjects/Player.hpp"
#include "Scoreboard.hpp"
#include "MiniScoreboard.hpp"
#include "../game/Match.hpp"

class HUD : public MonoBehaviour {
	Player *player = nullptr;
	Text *ammoText = nullptr, *reserveAmmoText = nullptr, *moneyText = nullptr, *hpText = nullptr, *armorText = nullptr,
		*primaryGun = nullptr, *secondaryGun = nullptr, *knife = nullptr, *weaponNameText = nullptr,
		*primaryGunBindText = nullptr, *secondaryGunBindText = nullptr, *knifeBindText = nullptr;
	Scoreboard *scoreboard = nullptr;
	MiniScoreboard *miniScoreboard = nullptr;

	SDL_Color color = { 0, 0, 0, 0 };
	SDL_Rect healthBarRect = { WINDOW_WIDTH / 2 - 267, WINDOW_HEIGHT - 20, 43, 4 };

public:
	HUD(SDL_Renderer *renderer, Player *player, Match *match);
	~HUD();

	void update();
	void render();
};