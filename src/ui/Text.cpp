#include "Text.hpp"

#include <stdexcept>

Text::Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color):
	MonoBehaviour(renderer), font(font), color(color)
{
	if (font == nullptr) {
		throw std::runtime_error("Font cannot be NULL");
	}
}

Text::~Text() {
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Text::set_content(std::string newContent) {
	if (content == newContent) {
		return;
	}

	content = newContent;

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	surface = TTF_RenderText_Solid(font, content.c_str(), color);

	if (surface == nullptr) {
		throw std::runtime_error("Failed to update text surface. Error: " + std::string(TTF_GetError()));
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == nullptr) {
		throw std::runtime_error("Failed to update text texture. Error: " + std::string(SDL_GetError()));
	}

	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

void Text::set_position(int x, int y) {
	rect.x = x, rect.y = y;
}

void Text::render() {
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
