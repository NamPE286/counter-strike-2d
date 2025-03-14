#pragma once

#include <SDL2/SDL.h>
#include "../gameObjects/Player.hpp"
#include "../behaviours/MonoBehaviour.hpp"

class PlayerCamera : public MonoBehaviour {
	SDL_Texture *texture = nullptr;
	SDL_Rect rect = { 0, 0, 0, 0 };
	Player *player = nullptr;

	int textH = 0, textW = 0;
public:
	PlayerCamera(SDL_Renderer *renderer, int w, int h, SDL_Texture* texture, Player* player);

	void update();
	void render();
};