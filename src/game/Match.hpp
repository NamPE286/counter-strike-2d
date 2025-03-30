#pragma once

#include <utility>
#include <vector>

#include "../gameObjects/Player.hpp"
#include "Map.hpp"

enum Reward {
	KILLALL,
	TIMEOUT,
	C4_EXPLODED,
	C4_DEFUSED,
	C4_PLANTED,
	C4_PLANTER,
	C4_DEFUSER
};

enum class Phase {
	WARMUP,
	BUY,
	PRE_PLANT,
	AFTER_PLANT,
	POST_ROUND
};

class Match {
	int maxRound = 4, maxPlayer = 10;
	int roundTime = 105, buyTime = 5, postRoundTime = 7;
	int reward[7] = { 3250, 3250, 3500, 3500, 800, 300, 300 };
	int lossBonus[6] = { 1900, 1400, 1900, 2400, 2900, 3400 };
	bool sideSwitched = false;

	void sort_players();

	void start_planting(Player *p);
	void start_defusing(Player *p, bool kit);

	void T_win(int rewardType);
	void CT_win(int rewardType);

	void start();
public:
	std::pair<int, int> scores = { 0, 0 };
	std::pair<int, int> alive = { 0, 0 };
	std::pair<int, int> lossInARow = { 0, 0 };
	std::vector<Player *> players;
	std::pair< std::vector<Player *>, std::vector<Player *>> team;
	Player *defuser = nullptr, *planter = nullptr;
	Phase phase = Phase::WARMUP;
	Map *map = nullptr;

	int round = 1, winner = PlayerSide::SPECTATOR;
	float timeLeft = 5, defuseTimer = 10, plantTimer = 3;
	bool planting = false, defusing = false;

	Match(SDL_Renderer *renderer, std::string mapName);
	~Match();

	void add_player(Player *player);
	void reset();

	// Return true if winner are decided
	bool update();
	void fixed_update();

	bool is_match_start_alert_visible() const;
	bool is_last_round_half_alert_visible() const;
	bool is_match_point_alert_visible() const;
	bool is_side_switched();

	void switch_side();
};