#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"

class Bullet : public MonoBehaviour {
	int alpha = 255;

public:
	float angle = 0;
	int x = 0, y = 0, length = 0;

	Bullet(SDL_Renderer* renderer);
	Bullet(SDL_Renderer* renderer, float angle, int x, int y, int length);

	void operator=(Bullet rhs);

	void update();
	void render();
	bool finished() const;
};