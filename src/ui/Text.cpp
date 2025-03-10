#include "Text.hpp"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color):
	renderer(renderer), font(font), color(color)
{}

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
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

void Text::set_position(int x, int y) {
	rect.x = x, rect.y = y;
}

void Text::render() {
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
