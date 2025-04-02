#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"
#include "../game/Match.hpp"
#include "../gameObjects/Player.hpp"
#include "../ui/Text.hpp"
#include "Alert.hpp"
#include "Announcer.hpp"
#include "Killfeed.hpp"
#include "MiniScoreboard.hpp"
#include "Scoreboard.hpp"

class HUD : public MonoBehaviour {
	Player *player = nullptr;
	Text *ammoText = nullptr, *reserveAmmoText = nullptr, *moneyText = nullptr, *hpText = nullptr, *armorText = nullptr,
		*primaryGun = nullptr, *secondaryGun = nullptr, *knife = nullptr, *weaponNameText = nullptr,
		*primaryGunBindText = nullptr, *secondaryGunBindText = nullptr, *knifeBindText = nullptr,
		*calloutText = nullptr;
	Scoreboard *scoreboard = nullptr;
	MiniScoreboard *miniScoreboard = nullptr;
	Match *match = nullptr;
	SDL_Color color = { 0, 0, 0, 0 };
	SDL_Rect healthBarRect = { WINDOW_WIDTH / 2 - 267, WINDOW_HEIGHT - 20, 43, 4 };

public:
	Alert *alert = nullptr;
	Announcer *announcer = nullptr;
	Killfeed *killfeed = nullptr;

	HUD(SDL_Renderer *renderer, Player *player, Match *match);
	~HUD();

	void update();
	void fixed_update();
	void update_callout(std::string s);
	void render();
};