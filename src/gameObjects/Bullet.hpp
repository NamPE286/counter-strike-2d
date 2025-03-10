#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"

class Bullet : public MonoBehaviour {
	float angle = 0;
	int x = 0, y = 0, alpha = 255, length = 0;

public:
	Bullet(SDL_Renderer* renderer, float angle, int x, int y, int length);
	void update();
	void render();
	bool finished() const;
};