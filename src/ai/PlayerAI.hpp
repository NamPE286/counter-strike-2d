#pragma once

#include <queue>

#include "../game/Map.hpp"
#include "../gameObjects/Player.hpp"
#include "../geometry/Vec2.hpp"

class PlayerAI {
	Map *map = nullptr;
	Player *p = nullptr;
	Vec2 dest = Vec2(-1, -1);
	std::queue<Vec2> path;

	void move(int x, int y);

public:
	PlayerAI(Map *map, Player *p);

	void update();
};