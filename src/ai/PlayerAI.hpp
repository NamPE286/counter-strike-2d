#pragma once

#include <utility>
#include <vector>

#include "../game/Match.hpp"
#include "../gameObjects/Player.hpp"
#include "../geometry/Vec2.hpp"

class PlayerAI {
	Match *match = nullptr;
	Player *p = nullptr;
	Vec2 dest = Vec2(-1, -1);
	bool moving = false;

	std::pair<int, int> get_direction(Vec2 a, Vec2 b);
	void move(int x, int y);
	void move_to(float x, float y);
	std::vector<Vec2> get_path(float x, float y);

public:
	PlayerAI(Match *match, Player *p);

	void update();
};