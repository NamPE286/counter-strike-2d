#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"

class Scoreboard : public MonoBehaviour {
	int w = 1000, h = 562;
	SDL_Texture *texture = nullptr;

public:
	Scoreboard(SDL_Renderer *renderer);
	~Scoreboard();

	void render();
};