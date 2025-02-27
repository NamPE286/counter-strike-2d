#pragma once

#include <SDL2/SDL.h>
#include <utility>
#include <map>

#include "../behaviours/MonoBehaviour.hpp"
#include "../geometry/Vec2.hpp"

class Player : public MonoBehaviour {
	std::map<Uint32, Vec2> directionMap = {
		{ SDLK_w, Vec2(0, -1) },
		{ SDLK_a, Vec2(-1, 0) },
		{ SDLK_s, Vec2(0, 1) },
		{ SDLK_d, Vec2(1, 0) }
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