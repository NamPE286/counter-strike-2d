#pragma once

#include <array>
#include <memory>
#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../common.h"
#include "../game/Match.hpp"
#include "Text.hpp"

class Scoreboard : public MonoBehaviour {
	int w = 1000, h = 562, padding = 20;
	SDL_Rect rect = {
		(WINDOW_WIDTH - w) / 2,
		(WINDOW_HEIGHT - h) / 2,
		w, h
	};
	Match *match = nullptr;
	Text *TScoreText = nullptr, *CTScoreText = nullptr,
		*TText = nullptr, *CTText = nullptr,
		*TAliveText = nullptr, *CTAliveText = nullptr,
		*infoText = nullptr, *timeElapsedText = nullptr;
	std::array<std::unique_ptr<Text>, 6> columnNameText;
	std::array<std::string, 6> columnName = { "", "Money", "Kills", "Deaths", "Assists", "Score" };
	std::array<int, 6> columnWidth = { 610, 70, 50, 50, 50, 50 };

	void render_column_name();
	void render_row(int x, int y, Player *p);

public:
	Scoreboard(SDL_Renderer *renderer, Match *match);
	~Scoreboard();

	void update();
	void render();
};