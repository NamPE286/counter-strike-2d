#pragma once

#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"
#include "Text.hpp"

class Announcer : public MonoBehaviour {
	Text *wonText = nullptr;
	Text *lostText = nullptr;
	SDL_Rect rect = { (WINDOW_WIDTH - 350) / 2, 120, 350, 80 };
	SDL_Color color = { 0, 0, 0, 0 };

public:
	Announcer(SDL_Renderer *renderer, SDL_Color color);

	void set_color(SDL_Color newColor);
	void render(bool win);
};