#include "Scoreboard.hpp"

#include "../managers/Font.hpp"

Scoreboard::Scoreboard(SDL_Renderer *renderer, Match *match):
	MonoBehaviour(renderer), match(match)
{
	infoText = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), { 180, 180, 180, 255 });
	infoText->set_content("Competitive | " + match->map->name);
	infoText->set_position(rect.x + padding, rect.y + padding);
}

Scoreboard::~Scoreboard() {
	delete infoText;
}

void Scoreboard::render() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 200);
	SDL_RenderFillRect(renderer, &rect);

	infoText->render();
}
