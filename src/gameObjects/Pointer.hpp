#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"

class Pointer : public MonoBehaviour {
	int x = 0, y = 0, w = 2, h = 16, gap = 6;
	int r = 0, g = 255, b = 0, a = 255;

public:
	Pointer(SDL_Renderer *renderer);

	void update();
	void render();
};