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

	timeElapsedText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 16), { 180, 180, 180, 255 });
}

Scoreboard::~Scoreboard() {
	delete infoText;
}

void Scoreboard::update() {
	int m = (int)std::floor(match->timeElapsed) / 60;
	int s = (int)match->timeElapsed % 60;

	timeElapsedText->set_content(Utils::getClockString(m, s));
	timeElapsedText->set_position(rect.x + rect.w - padding - timeElapsedText->rect.w, rect.y + padding);
}

void Scoreboard::render() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 200);
	SDL_RenderFillRect(renderer, &rect);

	infoText->render();
	timeElapsedText->render();

	int yOffset = rect.y + padding + infoText->rect.h;

	SDL_SetRenderDrawColor(renderer, 180, 180, 180, 100);
	SDL_RenderDrawLine(
		renderer,
		rect.x + padding,
		yOffset + 10,
		rect.x + padding + rect.w - 2 * padding,
		yOffset + 10);

	yOffset += 20;

}
