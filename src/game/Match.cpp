#include "Match.hpp"

#include <iostream>
#include <stdexcept>

#include "../managers/Time.hpp"


Match::Match(SDL_Renderer *renderer, std::string mapName) {
	map = new Map(renderer, "assets/tilemaps/" + mapName + ".tmx");
}

Match::~Match() {
	for (Player *p : players) {
		delete p;
	}

	delete map;
}

void Match::add_player(Player *player) {
	if (player->side == PlayerSide::T) {
		if (team.first.size() == maxPlayer) {
			throw std::runtime_error("Terrorist team is full");
		}

		team.first.push_back(player);
		players.push_back(player);
	} else if (player->side == PlayerSide::CT) {
		if (team.second.size() == maxPlayer) {
			throw std::runtime_error("Counter Terrorist team is full");
		}

		team.second.push_back(player);
		players.push_back(player);
	}
}

void Match::T_win(int rewardType) {
	winner = PlayerSide::T;

	scores.first++;
	lossInARow.first = 0;
	lossInARow.second = std::max(lossInARow.second + 1, 5);

	for (Player *p : team.first) {
		p->money += reward[rewardType];
	}

	for (Player *p : team.second) {
		if (round % (maxRound / 2) == 1) {
			p->money += lossBonus[0];
		} else {
			p->money += lossBonus[lossInARow.second];
		}
	}

	phase = Phase::POST_ROUND;
	timeLeft = (float)postRoundTime;
}

void Match::CT_win(int rewardType) {
	winner = PlayerSide::CT;

	scores.second++;
	lossInARow.first = std::max(lossInARow.first + 1, 5);
	lossInARow.second = 0;

	for (Player *p : team.second) {
		p->money += reward[rewardType];
	}

	for (Player *p : team.first) {
		if (rewardType == Reward::TIMEOUT && p->hp > 0) {
			continue;
		}

		if (round % (maxRound / 2) == 1) {
			p->money += lossBonus[0];
		} else {
			p->money += lossBonus[lossInARow.first];
		}
	}

	phase = Phase::POST_ROUND;
	timeLeft = (float)postRoundTime;
}

void Match::start() {
	for (Player *p : players) {
		p->hp = 100;
		p->money = 800;
		p->armor = 0;
		p->helmet = false;
		p->reset();
	}
}

void Match::reset() {
	for (Player *p : players) {
		p->reset();
	}
}

bool Match::update() {
	alive = { 0, 0 };

	for (Player *p : players) {
		if (p->side == PlayerSide::T && p->hp > 0) {
			alive.first++;
		} else if (p->side == PlayerSide::CT && p->hp > 0) {
			alive.second++;
		}
	}

	if (phase == Phase::PRE_PLANT) {
		if (timeLeft == 0.0f) {
			CT_win(Reward::TIMEOUT);
			return true;
		}

		if (alive.first == 0) {
			CT_win(Reward::KILLALL);
			return true;
		}

		if (alive.second == 0) {
			T_win(Reward::KILLALL);
			return true;
		}
	}

	if (phase == Phase::AFTER_PLANT) {
		if (defuseTimer == 0.0f) {
			defusing = false;
			defuser->money += reward[Reward::C4_DEFUSER];
			CT_win(Reward::C4_DEFUSED);
			return true;
		}

		if (timeLeft == 0.0f) {
			T_win(Reward::C4_EXPLODED);
			return true;
		}
	}

	return false;
}

void Match::fixed_update() {
	timeLeft -= Time::fixedDeltaTime / 1000;
	timeLeft = (float)std::max(0.0f, timeLeft);

	if (planting) {
		plantTimer -= Time::fixedDeltaTime / 1000;
		plantTimer = std::max(0.0f, plantTimer);

		if (plantTimer == 0.0f) {
			planting = false;
			phase = Phase::AFTER_PLANT;
		}
	}

	if (defusing) {
		defuseTimer -= Time::fixedDeltaTime / 1000;
		defuseTimer = std::max(0.0f, defuseTimer);
	}

	if (timeLeft == 0.0f) {
		if (phase == Phase::WARMUP) {
			start();

			phase = Phase::BUY;
			timeLeft = (float)buyTime;
		} else if (phase == Phase::BUY) {
			phase = Phase::PRE_PLANT;
			timeLeft = (float)roundTime;
		} else if (phase == Phase::POST_ROUND) {
			reset();

			phase = Phase::BUY;
			timeLeft = (float)buyTime;
		}
	}
}

void Match::start_planting(Player *p) {
	planter = p;
	planting = true;
	plantTimer = 3;
}

void Match::start_defusing(Player *p, bool kit) {
	defuser = p;
	defusing = true;

	if (kit) {
		defuseTimer = 5;
	} else {
		defuseTimer = 10;
	}
}