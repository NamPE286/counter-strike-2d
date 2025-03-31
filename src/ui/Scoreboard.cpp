#include "Scoreboard.hpp"

#include <cmath>

#include "../managers/Font.hpp"
#include "../utilities/Utils.hpp"

Scoreboard::Scoreboard(SDL_Renderer *renderer, Match *match):
	MonoBehaviour(renderer), match(match)
{
	infoText = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), { 180, 180, 180, 255 });
	infoText->set_content("Competitive | " + match->map->name);
	infoText->set_position(rect.x + padding, rect.y + padding);

	int xOffset = rect.x + padding;
	int yOffset = rect.y + padding + infoText->rect.h + 50;

	timeElapsedText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 16), { 180, 180, 180, 255 });
	TScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 50), TColor);
	CTScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 50), CTColor);

	TScoreText->set_position(xOffset + (50 - CTScoreText->rect.w) / 2, yOffset + 250);
	CTScoreText->set_position(xOffset + (50 - CTScoreText->rect.w) / 2, yOffset);
}

Scoreboard::~Scoreboard() {
	delete infoText;
}

void Scoreboard::update() {
	int m = (int)std::floor(match->timeElapsed) / 60;
	int s = (int)match->timeElapsed % 60;

	timeElapsedText->set_content(Utils::getClockString(m, s));
	timeElapsedText->set_position(rect.x + rect.w - padding - timeElapsedText->rect.w, rect.y + padding);
	TScoreText->set_content(std::to_string(match->scores.first));
	CTScoreText->set_content(std::to_string(match->scores.second));
}

void Scoreboard::render() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 200);
	SDL_RenderFillRect(renderer, &rect);

	infoText->render();
	timeElapsedText->render();
	TScoreText->render();
	CTScoreText->render();

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

}
