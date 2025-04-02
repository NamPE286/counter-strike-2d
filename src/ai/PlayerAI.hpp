#pragma once

#include <thread>
#include <utility>
#include <vector>

#include "../game/Match.hpp"
#include "../gameObjects/Player.hpp"
#include "../geometry/Vec2.hpp"

enum class AIState {
	ATTACKING,
	WANDERING
};

class PlayerAI {
	Match *match = nullptr;
	Player *p = nullptr, *target = nullptr;
	Vec2 dest = Vec2(-1, -1), pathDest = Vec2(-1, -1);
	std::thread movementThread, attackThread;
	bool moving = false, movingOnPath = false;
	bool stopped = false;
	AIState state = AIState::WANDERING;

	void move(int x, int y);
	void move_to(float x, float y);
	void align_position();
	void movement_thread_handler();
	void attack_thread_handler();
	bool can_target(Player *i);
	bool can_shoot(Player *i);

	std::pair<int, int> get_direction(Vec2 a, Vec2 b);
	std::vector<Vec2> optimize_path(std::vector<Vec2> &v);
	std::vector<Vec2> get_path(float x, float y);

public:
	PlayerAI(Match *match, Player *p);
	~PlayerAI();

	void update();
};