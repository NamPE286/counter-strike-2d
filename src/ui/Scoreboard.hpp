#pragma once

#include "../behaviours/MonoBehaviour.hpp"
#include "../game/Match.hpp"
#include <SDL2/SDL.h>

class Scoreboard : public MonoBehaviour {
	int w = 1000, h = 562;
	SDL_Texture *texture = nullptr;
	Match *match = nullptr;

public:
	Scoreboard(SDL_Renderer *renderer, Match *match);
	~Scoreboard();

	void render();
};