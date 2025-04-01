#include "PlayerAI.hpp"

#include <cmath>
#include <SDL2/SDL.h>
#include <thread>

std::pair<int, int> PlayerAI::get_direction(Vec2 a, Vec2 b) {
	int x = 0;
	int y = 0;

	if (b.x > a.x) {
		x = 1;
	} else if (b.x < a.x) {
		x = -1;
	}

	if (b.y > a.y) {
		y = 1;
	} else if (b.y < a.y) {
		y = -1;
	}

	return { x, y };
}

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

	moving = true;
}

void PlayerAI::move_to(float x, float y) {
	std::thread t([&]() {
		auto path = get_path(x, y);

		for (Vec2 &i : path) {
			while (moving) {
				continue;
			}

			dest = i;
			auto dir = get_direction(p->position, i);

			move(dir.first, dir.second);
		}
	});

	t.detach();
}

std::vector<Vec2> PlayerAI::optimize_path(std::vector<Vec2> &v) {
	std::vector<Vec2> res;
	std::pair<int, int> dir;

	for (Vec2 &i : v) {
		std::pair<int, int> tmp;

		if (res.empty()) {
			tmp = get_direction(p->position, i);
		} else {
			tmp = get_direction(res.back(), i);
		}

		if (tmp == dir) {
			res.pop_back();
		}

		res.push_back(i);
		dir = tmp;
	}

	return res;
}

std::vector<Vec2> PlayerAI::get_path(float x, float y) {
	std::vector<Vec2> res = { p->position + Vec2(0, 32), p->position + Vec2(0, 32 * 2), p->position + Vec2(0, 32 * 3), p->position + Vec2(32, 32 * 3), p->position + Vec2(32 * 2, 32 * 3), p->position + Vec2(32 * 3, 32 * 3) };
	return optimize_path(res);
}

PlayerAI::PlayerAI(Match *match, Player *p):
	match(match), p(p)
{
	auto *tile = match->map->get_tile((int)p->position.x, (int)p->position.y);

	p->set_position(Vec2(
		std::floor(p->position.x / (float)match->map->map->tile_width) * (float)match->map->map->tile_width + (float)match->map->map->tile_width / 2,
		std::floor(p->position.y / (float)match->map->map->tile_height) * (float)match->map->map->tile_height + (float)match->map->map->tile_height / 2
	));

	move_to(1, 1);

	dest = p->position + Vec2(32, 32);
}

void PlayerAI::update() {
	if ((dest + p->position * -1).magnitude() < 1.0f) {
		p->stop_movement();
		p->position = dest;
		moving = false;
	}

	if (p->velocity.magnitude() == 0) {
		p->set_position(Vec2(
			std::floor(p->position.x / (float)match->map->map->tile_width) * (float)match->map->map->tile_width + (float)match->map->map->tile_width / 2,
			std::floor(p->position.y / (float)match->map->map->tile_height) * (float)match->map->map->tile_height + (float)match->map->map->tile_height / 2
		));
	}
}
