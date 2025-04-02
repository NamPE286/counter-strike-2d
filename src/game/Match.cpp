#include "Match.hpp"

#include <algorithm>
#include <stdexcept>

#include "../managers/Audio.hpp"
#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"

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
	bonusLevel.first = 0;
	bonusLevel.second = std::min(bonusLevel.second + 1, 4);

	for (Player *p : team.first) {
		p->money += reward[rewardType];
	}

	for (Player *p : team.second) {
		p->money += lossBonus[bonusLevel.second];
	}

	phase = Phase::POST_ROUND;
	timeLeft = (float)postRoundTime;
}

void Match::CT_win(int rewardType) {
	winner = PlayerSide::CT;

	scores.second++;
	bonusLevel.first = std::min(bonusLevel.first + 1, 4);
	bonusLevel.second = 0;

	for (Player *p : team.second) {
		p->money += reward[rewardType];
	}

	for (Player *p : team.first) {
		if (rewardType == Reward::TIMEOUT && p->hp > 0) {
			continue;
		}

		p->money += lossBonus[bonusLevel.first];
	}

	phase = Phase::POST_ROUND;
	timeLeft = (float)postRoundTime;
}

void Match::start() {
	timeElapsed = 0.0;

	for (Player *p : players) {
		p->money = 800;
		p->armor = p->kill = p->death = p->assist = 0;
		p->helmet = false;
	}

	reset();
}

void Match::reset() {
	for (Player *p : players) {
		p->reset();
	}

	winner = PlayerSide::SPECTATOR;
	planting = defusing = false;

	sort_players();
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
	timeLeft = std::max(0.0f, timeLeft);
	timeElapsed += Time::fixedDeltaTime / 1000;

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
			if (round == maxRound / 2) {
				switch_side();
				sideSwitched = true;
			}

			reset();

			round++;
			phase = Phase::BUY;
			timeLeft = (float)buyTime;

			if (std::max(scores.first, scores.second) == maxRound / 2) {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/cs_stinger.wav"), 0);
			} else if (round == maxRound / 2) {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/lastroundhalf.wav"), 0);
			}
		}
	}
}

bool Match::is_match_start_alert_visible() const {
	return ((int)timeLeft >= (roundTime - 3)) && (round == 1) && (phase == Phase::PRE_PLANT);
}

bool Match::is_last_round_half_alert_visible() const {
	return (phase == Phase::BUY) && (timeLeft >= buyTime - 3) && (round == (maxRound / 2));
}

bool Match::is_match_point_alert_visible() const {
	return (phase == Phase::BUY) && (timeLeft >= buyTime - 3) && (std::max(scores.first, scores.second) == maxRound / 2);
}

bool Match::is_side_switched() {
	if (!sideSwitched) {
		return false;
	}

	sideSwitched = false;

	return true;
}

void Match::switch_side() {
	for (Player *p : players) {
		p->switch_side();
	}

	std::swap(scores.first, scores.second);
	std::swap(team.first, team.second);

	bonusLevel = { 1, 1 };
}

void Match::sort_players() {
	std::sort(players.begin(), players.end(), [](Player *a, Player *b) {
		return (a->kill * 2 + a->assist) < (b->kill * 2 + b->assist);
	});
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