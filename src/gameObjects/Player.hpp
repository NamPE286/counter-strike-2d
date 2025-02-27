#pragma once

#include <SDL2/SDL.h>
#include <utility>
#include <vector>

#include "../behaviours/MonoBehaviour.hpp"
#include "../geometry/Vec2.hpp"

class Player : public MonoBehaviour {
	std::vector<std::pair<Uint32, Vec2>> directionBind = {
		{ SDL_SCANCODE_W, Vec2(0, -1) },
		{ SDL_SCANCODE_A, Vec2(-1, 0) },
		{ SDL_SCANCODE_S, Vec2(0, 1) },
		{ SDL_SCANCODE_D, Vec2(1, 0) }
	};
	Vec2 position, velocity;
	const int size = 30, borderWidth = 8;
	const float speed = 0.2f;
	int r, g, b;

public:
	Player(SDL_Renderer* renderer, int r, int g, int b, Vec2 pos = Vec2(0, 0));

	void update();
	void fixed_update();
	void render();
	void on_key_down(SDL_Event& event);
	void on_key_up(SDL_Event& event);
};