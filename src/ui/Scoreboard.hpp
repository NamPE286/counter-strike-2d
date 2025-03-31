#pragma once

#include <SDL2/SDL.h>

#include "../common.h"
#include "../behaviours/MonoBehaviour.hpp"
#include "../game/Match.hpp"
#include "Text.hpp"

class Scoreboard : public MonoBehaviour {
	int w = 1000, h = 562, padding = 20;
	SDL_Rect rect = {
		(WINDOW_WIDTH - w) / 2,
		(WINDOW_HEIGHT - h) / 2,
		w, h
	};
	Match *match = nullptr;
	Text *TScoreText = nullptr, *CTScoreText = nullptr,
		*TText = nullptr, *CTText = nullptr,
		*infoText = nullptr, *timeElapsedText = nullptr;

public:
	Scoreboard(SDL_Renderer *renderer, Match *match);
	~Scoreboard();

	void update();
	void render();
};