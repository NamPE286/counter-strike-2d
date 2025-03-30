#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../game/Match.hpp"
#include "Text.hpp"

class Scoreboard : public MonoBehaviour {
	int w = 1000, h = 562;
	SDL_Texture *texture = nullptr;
	Match *match = nullptr;
	Text *TScoreText = nullptr, *CTScoreText = nullptr, *infoText = nullptr, *timeElapsed = nullptr;

public:
	Scoreboard(SDL_Renderer *renderer, Match *match);
	~Scoreboard();

	void render();
};