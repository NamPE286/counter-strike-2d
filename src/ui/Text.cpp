#include "Text.hpp"

#include <stdexcept>

Text::Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color):
	MonoBehaviour(renderer), font(font), color(color)
{
	if (font == nullptr) {
		throw std::runtime_error("Font cannot be NULL");
	}
}

Text::~Text() {
	if (surface) {
		SDL_FreeSurface(surface);
	}

	SDL_DestroyTexture(texture);
}

bool Text::empty() {
	return content.empty();
}

void Text::set_content(std::string newContent) {
	if (newContent.empty()) {
		content.clear();

		return;
	}

	if (content == newContent) {
		return;
	}

	content = newContent;

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	surface = TTF_RenderText_Blended(font, content.c_str(), color);

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

void Text::center(int w) {
	rect.x += (w - rect.w) / 2;
}

void Text::render() {
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
