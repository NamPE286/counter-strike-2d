#include "Text.hpp"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, std::string content, SDL_Color color):
	renderer(renderer), font(font), color(color)
{
	update_content(content);
}

void Text::update_content(std::string newContent) {
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

void Text::render(int x, int y) {
	rect.x = x, rect.y = y;

	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
