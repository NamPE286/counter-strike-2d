#include "PlayerCamera.hpp"

#include "../managers/Mouse.hpp"
#include "../common.h"

PlayerCamera::PlayerCamera(SDL_Renderer *renderer, int w, int h, SDL_Texture *texture, Player *player):
	MonoBehaviour(renderer), texture(texture), player(player)
{
	rect.w = w, rect.h = h;
	SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
}

void PlayerCamera::update() {
	rect.x = std::min(textW - rect.w, std::max(0, (int)player->position.x - rect.w / 2));
	rect.y = std::min(textH - rect.h, std::max(0, (int)player->position.y - rect.h / 2));

	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);
	Mouse::update(rect.x + x * rect.w / WINDOW_WIDTH, rect.y + y * rect.h / WINDOW_HEIGHT);
}

void PlayerCamera::render() {
	SDL_RenderCopy(renderer, texture, &rect, nullptr);
}