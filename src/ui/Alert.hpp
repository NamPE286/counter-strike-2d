#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "Text.hpp"
#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"

class Alert : public MonoBehaviour {
	Text *text = nullptr;
	SDL_Rect rect = { (WINDOW_WIDTH - 250) / 2, WINDOW_HEIGHT - 200, 250, 35 };
	SDL_Color color = { 0, 0, 0, 0 };
public:
	Alert(SDL_Renderer *renderer, SDL_Color color);

	void set_content(std::string content);
	void set_color(SDL_Color newColor);
	void render();
};