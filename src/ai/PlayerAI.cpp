#include "PlayerAI.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <random>
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
	pathDest = Vec2(x * match->map->map->tile_width + match->map->map->tile_width / 2, y * match->map->map->tile_height + match->map->map->tile_height / 2);

	auto tmp = pathDest;
	align_position();
	auto path = get_path(pathDest.x, pathDest.y);

	movingOnPath = true;

	for (Vec2 &i : path) {
		while (moving) {
			continue;
		}

		if (!movingOnPath) {
			break;
		}

		dest = i;
		auto dir = get_direction(p->position, i);

		align_position();
		move(dir.first, dir.second);
	}

	movingOnPath = false;
}
void PlayerAI::align_position() {
	p->set_position(Vec2(
		std::floor(p->position.x / (float)match->map->map->tile_width) * (float)match->map->map->tile_width + (float)match->map->map->tile_width / 2,
		std::floor(p->position.y / (float)match->map->map->tile_height) * (float)match->map->map->tile_height + (float)match->map->map->tile_height / 2
	));
}

void PlayerAI::movement_thread_handler() {
	while (!stopped) {
		if (match->phase == Phase::BUY) {
			continue;
		}

		if (movingOnPath || state != AIState::WANDERING) {
			continue;
		}

		auto pos = match->map->get_random_position();

		//move_to((float)pos.first, (float)pos.second);

		if (match->round == 1) {
			move_to(48, 52);
		} else if (match->round == 2) {
			move_to(74, 47);
		}

		SDL_Delay(1500);
	}
}

void PlayerAI::attack_thread_handler() {
	while (!stopped) {
		if (target != nullptr) {
			if (can_shoot(target)) {
				movingOnPath = moving = false;

				p->stop_movement();
				SDL_Delay(200);
				p->fire();
				SDL_Delay(300);
				p->stop_firing();
				SDL_Delay(500);
			}

			if (!can_target(target)) {
				SDL_Delay(1000);

				if (!can_target(target)) {
					target = nullptr;
				}
			}
		} else {
			Player *tmp = nullptr;

			for (Player *i : match->players) {
				if (i == p || i->side == p->side) {
					continue;
				}

				if (can_target(i)) {
					if (tmp == nullptr) {
						tmp = i;
					} else if ((p->position + i->position * -1).magnitude() < (p->position + tmp->position * -1).magnitude()) {
						tmp = i;
					}
				}
			}

			if (tmp != nullptr) {
				target = tmp;
				movingOnPath = moving = false;
				state = AIState::ATTACKING;

				p->set_fire_target(&target->position);
				p->stop_movement();
				align_position();
				moving = false;
				SDL_Delay(200);

				std::thread t([this]() {
					move_to(std::floor(target->position.x / 32), std::floor(target->position.y / 32));
				});

				t.detach();
			} else {
				state = AIState::WANDERING;
			}
		}
	}
}

void PlayerAI::stop() {
	p->stop_movement();
	p->stop_firing();
	align_position();
	moving = movingOnPath = false;
}

bool PlayerAI::can_target(Player *i) {
	return i != nullptr &&
		match->map->is_visible(p->position, i->position) &&
		(p->position + i->position * -1).magnitude() < 500.0f &&
		i->hp > 0;
}

bool PlayerAI::can_shoot(Player *i) {
	return i != nullptr &&
		match->map->is_visible(p->position, i->position) &&
		(p->position + i->position * -1).magnitude() < std::min(p->get_weapon()->range, 250.0f) &&
		i->hp > 0;
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
	struct Item {
		std::pair<int, int> pos, dir;
		int step = 0, turn = 0;

		bool operator<(const Item &y) const {
			if (step == y.step) {
				return turn < y.turn;
			}

			return step > y.step;
		}
	};

	std::vector<std::pair<int, int>> dirs = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	std::map<std::pair<int, int>, std::pair<int, int>> parent;
	std::set<std::pair<int, int>> visited;
	std::priority_queue<Item> pq;
	std::pair<int, int> end = { (int)x, (int)y };

	align_position();
	pq.push({ { (int)p->position.x, (int)p->position.y }, { 0, 0 }, 0, 0 });
	visited.emplace((int)p->position.x, (int)p->position.y);

	auto rng = std::default_random_engine{};
	std::shuffle(dirs.begin(), dirs.end(), rng);

	while (!pq.empty()) {
		Item cur = pq.top();
		auto &[pos, dir, turn, step] = cur;
		pq.pop();

		if (pos == end) {
			break;
		}

		for (auto &i : dirs) {
			auto &[x, y] = i;
			std::pair<int, int> tmp = {
				pos.first + x * match->map->map->tile_width,
				pos.second + y * match->map->map->tile_height
			};
			auto tile = match->map->get_tile(tmp.first, tmp.second);

			if (tile && tile->collision) {
				continue;
			}

			if (visited.contains(tmp)) {
				continue;
			}

			visited.insert(tmp);
			parent[tmp] = pos;

			pq.push({ tmp, i, step + 1, turn + (i == dir) });
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
	movementThread = std::thread([this]() {
		movement_thread_handler();
		});

	attackThread = std::thread([this]() {
		attack_thread_handler();
		});
}

PlayerAI::~PlayerAI() {
	stopped = true;
	moving = movingOnPath = false;

	movementThread.join();
	attackThread.join();
}

void PlayerAI::update() {
	if (match->phase == Phase::BUY) {
		stop();
		state = AIState::WANDERING;
	}

	if ((dest + p->position * -1).magnitude() < 1.0f) {
		p->stop_movement();
		p->stop_firing();
		align_position();
		moving = false;
		p->position = dest;
	}
}
