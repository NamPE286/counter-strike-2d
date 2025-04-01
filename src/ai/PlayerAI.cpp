#include "PlayerAI.hpp"

#include <SDL2/SDL.h>
#include <cmath>

void PlayerAI::move(int x, int y) {
	if (x == 1) {
		SDL_Event e{};
		e.key.keysym.scancode = SDL_SCANCODE_D;

		p->on_key_down(e);
	} else if (x == -1) {
		SDL_Event e{};
		e.key.keysym.scancode = SDL_SCANCODE_A;

		p->on_key_down(e);
	}

	if (y == 1) {
		SDL_Event e{};
		e.key.keysym.scancode = SDL_SCANCODE_S;

		p->on_key_down(e);
	} else if (y == -1) {
		SDL_Event e{};
		e.key.keysym.scancode = SDL_SCANCODE_W;

		p->on_key_down(e);
	}
}

PlayerAI::PlayerAI(Map *map, Player *p):
	map(map), p(p)
{
	auto *tile = map->get_tile((int)p->position.x, (int)p->position.y);

	p->set_position(Vec2(
		std::floor(p->position.x / (float)map->map->tile_width) * (float)map->map->tile_width + (float)map->map->tile_width / 2,
		std::floor(p->position.y / (float)map->map->tile_height) * (float)map->map->tile_height + (float)map->map->tile_height / 2
	));

	move(1, 1);

	dest = p->position + Vec2(32, 32);
}

void PlayerAI::update() {
	if ((dest + p->position * -1).magnitude() < 1.0f) {
		p->stop_movement();
		p->position = dest;
	}

	if (p->velocity.magnitude() == 0) {
		p->set_position(Vec2(
			std::floor(p->position.x / (float)map->map->tile_width) * (float)map->map->tile_width + (float)map->map->tile_width / 2,
			std::floor(p->position.y / (float)map->map->tile_height) * (float)map->map->tile_height + (float)map->map->tile_height / 2
		));
	}
}
