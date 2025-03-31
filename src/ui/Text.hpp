#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../behaviours/MonoBehaviour.hpp"

class Text : public MonoBehaviour {
	SDL_Surface *surface = nullptr;
	SDL_Texture *texture = nullptr;
	TTF_Font *font = nullptr;
	SDL_Color color = { 0, 0, 0, 0 };
	std::string content;

public:
	SDL_Rect rect = { 0, 0, 0, 0 };

	Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
	~Text();

	bool empty();
	void set_content(std::string newContent);
	void set_position(int x, int y);
	void align_center(int w);
	void align_right(int w);
	void render();
};