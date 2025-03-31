#include "Scoreboard.hpp"

#include <cmath>

#include "../managers/Font.hpp"
#include "../utilities/Utils.hpp"

void Scoreboard::render_column_name() {
	for (auto &i : columnNameText) {
		i->render();
	}
}

void Scoreboard::render_row(int x, int y, Player *p) {
	for (size_t i = 0; i < columnWidth.size(); i++) {
		SDL_Rect r = { x, y, columnWidth[i], 20 };
		SDL_Color color = { 255, 255, 255, 255 };

		if (p->side == PlayerSide::T) {
			color = TColor;
		} else if (p->side == PlayerSide::CT) {
			color = CTColor;
		}

		Text *text = text = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 16), color);

		if (i == columnWidth.size() - 1) {
			SDL_SetRenderDrawColor(renderer, 80, 80, 80, 200);
		} else if (i % 2 == 0) {
			SDL_SetRenderDrawColor(renderer, 48, 48, 48, 200);
		} else {
			SDL_SetRenderDrawColor(renderer, 60, 60, 60, 200);
		}

		SDL_RenderFillRect(renderer, &r);
		if (columnName[i] == "") {
			text->set_content(p->name);
			text->set_position(x, y);
			text->rect.x += 20;
		} else if (columnName[i] == "Money") {
			text->set_content("$" + std::to_string(p->money));
			text->set_position(x, y);
			text->align_right(columnWidth[i]);
			text->rect.x -= 5;
		} else if (columnName[i] == "Kills") {
			text->set_content(std::to_string(p->kill));
			text->set_position(x, y);
			text->align_center(columnWidth[i]);
		} else if (columnName[i] == "Deaths") {
			text->set_content(std::to_string(p->death));
			text->set_position(x, y);
			text->align_center(columnWidth[i]);
		} else if (columnName[i] == "Assists") {
			text->set_content(std::to_string(p->assist));
			text->set_position(x, y);
			text->align_center(columnWidth[i]);
		} else if (columnName[i] == "Scores") {
			text->set_content(std::to_string(p->kill * 2 + p->assist));
			text->set_position(x, y);
			text->align_center(columnWidth[i]);
		}

		text->render();

		x += columnWidth[i];
		delete text;
	}
}

Scoreboard::Scoreboard(SDL_Renderer *renderer, Match *match):
	MonoBehaviour(renderer), match(match)
{
	infoText = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), { 180, 180, 180, 255 });
	infoText->set_content("Competitive | " + match->map->name);
	infoText->set_position(rect.x + padding, rect.y + padding);

	int xOffset = rect.x + padding;
	int yOffset = rect.y + padding + infoText->rect.h + 70;

	timeElapsedText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 16), { 180, 180, 180, 255 });
	TScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 55), TColor);
	CTScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 55), CTColor);

	TScoreText->set_position(xOffset + 15, yOffset + 250);
	CTScoreText->set_position(xOffset + 15, yOffset);

	TText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 16), TColor);
	TText->set_content("Terrorist");
	TText->set_position(xOffset + 28 - (TText->rect.w) / 2, yOffset + 300 + TScoreText->rect.h + 15);

	CTText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 16), CTColor);
	CTText->set_content("Counter-Ter");
	CTText->set_position(xOffset + 28 - (CTText->rect.w) / 2, yOffset + 50 + CTScoreText->rect.h + 15);

	TAliveText = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), TColor);
	CTAliveText = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), CTColor);

	xOffset += 80;
	yOffset -= 10;

	int x = xOffset;

	for (size_t i = 0; i < columnName.size(); i++) {
		columnNameText[i] = std::unique_ptr<Text>(new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 12), { 180, 180, 180, 255 }));
		columnNameText[i]->set_content(columnName[i]);
		columnNameText[i]->set_position(x, yOffset);
		columnNameText[i]->align_center(columnWidth[i]);

		x += columnWidth[i];
	}
}

Scoreboard::~Scoreboard() {
	delete infoText;
	delete TScoreText;
	delete CTScoreText;
	delete TText;
	delete CTText;
	delete timeElapsedText;
	delete TAliveText;
	delete CTAliveText;
}

void Scoreboard::update() {
	int m = (int)std::floor(match->timeElapsed) / 60;
	int s = (int)match->timeElapsed % 60;
	int xOffset = rect.x + padding;
	int yOffset = rect.y + padding + infoText->rect.h + 70;

	timeElapsedText->set_content(Utils::getClockString(m, s));
	timeElapsedText->set_position(rect.x + rect.w - padding - timeElapsedText->rect.w, rect.y + padding);

	TScoreText->set_content(std::to_string(match->scores.first));
	CTScoreText->set_content(std::to_string(match->scores.second));

	TAliveText->set_content("Alive " + std::to_string(match->alive.first) + "/" + std::to_string(match->team.first.size()));
	CTAliveText->set_content("Alive " + std::to_string(match->alive.second) + "/" + std::to_string(match->team.second.size()));

	TAliveText->set_position(TScoreText->rect.x - 12, TScoreText->rect.y + TScoreText->rect.h + TText->rect.h + 10);
	CTAliveText->set_position(CTScoreText->rect.x - 12, CTScoreText->rect.y + CTScoreText->rect.h + CTText->rect.h + 10);
}

void Scoreboard::render() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 200);
	SDL_RenderFillRect(renderer, &rect);

	infoText->render();
	timeElapsedText->render();
	TScoreText->render();
	CTScoreText->render();
	TText->render();
	CTText->render();
	TAliveText->render();
	CTAliveText->render();

	int xOffset = rect.x + padding;
	int yOffset = rect.y + padding + infoText->rect.h;

	SDL_SetRenderDrawColor(renderer, 180, 180, 180, 100);
	SDL_RenderDrawLine(
		renderer,
		xOffset,
		yOffset + 10,
		xOffset + rect.w - 2 * padding,
		yOffset + 10);

	yOffset += 50;
	xOffset += 80;

	render_column_name();

	yOffset += 25;

	for (Player *p : match->team.second) {
		render_row(xOffset, yOffset, p);

		yOffset += 22;
	}

	yOffset = 430;

	for (Player *p : match->team.first) {
		render_row(xOffset, yOffset, p);

		yOffset += 22;
	}
}
