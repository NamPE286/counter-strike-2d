#pragma once

#include "../gameObjects/Player.hpp"
#include "../geometry/Vec2.hpp"

class PlayerAI {
	Player *p = nullptr;
	Vec2 dest = Vec2(-1, -1);

public:
	PlayerAI(Player *p);

	void update();
};