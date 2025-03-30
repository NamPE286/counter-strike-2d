#include "Scoreboard.hpp"

#include <stdexcept>

#include "../common.h"

Scoreboard::Scoreboard(SDL_Renderer *renderer, Match *match):
	MonoBehaviour(renderer), match(match)
{
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	if (!texture) {
		throw std::runtime_error("Failed to create scoreboard texture");
	}

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

Scoreboard::~Scoreboard() {
	SDL_DestroyTexture(texture);
}

void Scoreboard::render() {
	SDL_SetRenderTarget(renderer, texture);

	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 200);
	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
	
	SDL_Rect rect = {
		(WINDOW_WIDTH - w) / 2,
		(WINDOW_HEIGHT - h) / 2,
		w, h
	};

	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
