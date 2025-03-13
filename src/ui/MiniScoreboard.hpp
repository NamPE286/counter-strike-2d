#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"
#include "../game/Match.hpp"
#include "../common.h"

class MiniScoreboard : public MonoBehaviour {
	int w = 300, h = 100;
	Match *match = nullptr;

public:
	MiniScoreboard(SDL_Renderer *renderer, Match *match);

	void update();
	void render();
};