#include "Alert.hpp"

#include "../managers/Font.hpp"

Alert::Alert(SDL_Renderer *renderer, SDL_Color color):
	MonoBehaviour(renderer), color(color)
{
	text = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 16), { 255, 255, 255, 255 });
}

void Alert::set_content(std::string content) {
	text->set_content(content);
	text->set_position((WINDOW_WIDTH - text->rect.w) / 2, rect.y + (rect.h - text->rect.h) / 2);
}

void Alert::render() {
	if (text->empty()) {
		return;
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

	SDL_Rect vertBar1 = { rect.x, rect.y, 3, rect.h };
	SDL_Rect vertBar2 = { rect.x + rect.w - 3, rect.y, 3, rect.h };

	SDL_RenderFillRect(renderer, &vertBar1);
	SDL_RenderFillRect(renderer, &vertBar2);

	text->render();
}
