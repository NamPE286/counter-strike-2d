#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../geometry/Vec2.hpp"

class Player : public MonoBehaviour {
	Vec2 pos, velocity;

public:
	Player(SDL_Renderer* renderer, Vec2 pos = Vec2(0, 0));

	void render();
};