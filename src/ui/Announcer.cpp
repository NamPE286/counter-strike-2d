#include "Announcer.hpp"

#include <SDL2/SDL.h>
#include <string>

#include "../managers/Font.hpp"

Announcer::Announcer(SDL_Renderer *renderer, SDL_Color color):
	MonoBehaviour(renderer), color(color)
{
	wonText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 50), color);
	wonText->set_content("ROUND WON");
	wonText->set_position((WINDOW_WIDTH - wonText->rect.w) / 2, rect.y + (rect.h - wonText->rect.h) / 2);

	lostText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 50), RedColor);
	lostText->set_content("ROUND LOST");
	lostText->set_position((WINDOW_WIDTH - wonText->rect.w) / 2, rect.y + (rect.h - wonText->rect.h) / 2);
}

void Announcer::set_color(SDL_Color newColor) {
	color = newColor;
}

void Announcer::render(bool win) {
	if (win) {
		SDL_SetRenderDrawColor(renderer, color.r / 4, color.g / 4, color.b / 4, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

		SDL_Rect vertBar1 = { rect.x, rect.y, 3, rect.h };
		SDL_Rect vertBar2 = { rect.x + rect.w - 3, rect.y, 3, rect.h };

		SDL_RenderFillRect(renderer, &vertBar1);
		SDL_RenderFillRect(renderer, &vertBar2);

		wonText->render();
	} else {
		SDL_SetRenderDrawColor(renderer, RedColor.r / 4, RedColor.g / 4, RedColor.b / 4, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, RedColor.r, RedColor.g, RedColor.b, 255);

		SDL_Rect vertBar1 = { rect.x, rect.y, 3, rect.h };
		SDL_Rect vertBar2 = { rect.x + rect.w - 3, rect.y, 3, rect.h };

		SDL_RenderFillRect(renderer, &vertBar1);
		SDL_RenderFillRect(renderer, &vertBar2);

		lostText->render();
	}
}
