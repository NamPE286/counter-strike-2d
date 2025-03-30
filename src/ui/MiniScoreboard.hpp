#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"
#include "../game/Match.hpp"
#include "../ui/Text.hpp"

class MiniScoreboard : public MonoBehaviour {
	Match *match = nullptr;

	Text *timerText = nullptr, *TScoreText = nullptr, *CTScoreText = nullptr, *TAliveText = nullptr, *CTAliveText = nullptr;
	Text *aliveT = nullptr, *aliveCT = nullptr;

	SDL_Rect timerRect = { WINDOW_WIDTH / 2 - 40, 0, 80, 30 };
	SDL_Rect TScoreRect = { WINDOW_WIDTH / 2 + 1, 32, 39, 30 };
	SDL_Rect CTScoreRect = { WINDOW_WIDTH / 2 - 40, 32, 39, 30 };
	SDL_Rect TAliveRect = { WINDOW_WIDTH / 2 + 40 + 2, 0, 62, 62 };
	SDL_Rect CTAliveRect = { WINDOW_WIDTH / 2 - 104, 0, 62, 62 };

public:
	MiniScoreboard(SDL_Renderer *renderer, Match *match);

	void update();
	void render();
};