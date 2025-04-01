#include "PlayerAI.hpp"

#include <cmath>
#include <map>
#include <queue>
#include <SDL2/SDL.h>
#include <set>
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
	std::thread t([this, x, y]() {
		dest = Vec2(x * match->map->map->tile_width + match->map->map->tile_width / 2, y * match->map->map->tile_height + match->map->map->tile_height / 2);
		auto path = get_path(dest.x, dest.y);

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

void PlayerAI::align_position() {
	p->set_position(Vec2(
		std::floor(p->position.x / (float)match->map->map->tile_width) * (float)match->map->map->tile_width + (float)match->map->map->tile_width / 2,
		std::floor(p->position.y / (float)match->map->map->tile_height) * (float)match->map->map->tile_height + (float)match->map->map->tile_height / 2
	));
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
	std::vector<std::pair<int, int>> dirs = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	std::map<std::pair<int, int>, std::pair<int, int>> parent;
	std::set<std::pair<int, int>> visited;
	std::queue<std::pair<int, int>> q;
	std::pair<int, int> end = { (int)x, (int)y };
	
	align_position();
	q.emplace((int)p->position.x, (int)p->position.y);
	visited.emplace((int)p->position.x, (int)p->position.y);

	while (!q.empty()) {
		std::pair<int, int> cur = q.front();
		q.pop();

		if (cur == end) {
			break;
		}

		for (auto &[x, y] : dirs) {
			std::pair<int, int> tmp = {
				cur.first + x * match->map->map->tile_width,
				cur.second + y * match->map->map->tile_height
			};
			auto tile = match->map->get_tile(tmp.first, tmp.second);

			if (tile && tile->collision) {
				continue;
			}

			if (visited.contains(tmp)) {
				continue;
			}

			visited.insert(tmp);
			parent[tmp] = cur;

			q.push(tmp);
		}
	}

	std::vector<std::pair<int, int>> tmp = { end };
	
	while (parent.contains(tmp.back())) {
		tmp.push_back(parent[tmp.back()]);
	}

	tmp.pop_back();

	std::vector<Vec2> res;

	for (int i = (int)tmp.size() - 1; i >= 0; i--) {
		res.push_back(Vec2((float)tmp[i].first, (float)tmp[i].second));
	}

	return optimize_path(res);
}

PlayerAI::PlayerAI(Match *match, Player *p):
	match(match), p(p)
{
	auto *tile = match->map->get_tile((int)p->position.x, (int)p->position.y);

	align_position();
	move_to(50, 70);
}

void PlayerAI::update() {
	if ((dest + p->position * -1).magnitude() < 1.0f) {
		p->stop_movement();
		p->position = dest;
		moving = false;
	}

	if (p->velocity.magnitude() == 0) {
		align_position();
	}
}
