#pragma once

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"
#include <SDL2/SDL.h>

class PlayerCamera : public MonoBehaviour {
	SDL_Texture *texture = nullptr;
	SDL_Rect rect = { 0, 0, 0, 0 };

	int textH = 0, textW = 0;
public:
	Player *target = nullptr;

	PlayerCamera(SDL_Renderer *renderer, int w, int h, SDL_Texture* texture, Player* player);

	void update();
	void render();
};