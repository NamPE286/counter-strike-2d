#include "Killfeed.hpp"

#include <iostream>

#include "../common.h"
#include "../managers/Font.hpp"
#include "../managers/Time.hpp"

Killfeed::Killfeed(SDL_Renderer *renderer):
	MonoBehaviour(renderer) 
{}

void Killfeed::add(Player *from, Player *to, bool headshot) {
	Item item = {
		new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 15), from->color),
		new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 15), to->color),
		new Text(renderer, Font::load("assets/fonts/icon.ttf", 22), { 255, 255, 255, 255 }),
		10000.0f,
		from->playable
	};

	item.from->set_content(from->name);
	item.to->set_content(to->name);
	item.sym->set_content(std::string{ from->get_weapon()->symbol });

	feed.push_back(item);

	std::cout << from->name << " killed " << to->name << ' ' << (headshot ? "by headshot " : " ") << "with " << from->get_weapon()->name << '\n';
}

void Killfeed::render() {
	for (auto &i : feed) {
		SDL_Rect r = {
			i.from->rect.x - 12,
			i.from->rect.y - 5,
			i.from->rect.w + i.sym->rect.w + i.to->rect.w + 44,
			30
		};

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 140);
		SDL_RenderFillRect(renderer, &r);

		i.from->render();
		i.sym->render();
		i.to->render();

		if (!i.isSelf) {
			return;
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &r);
	}
}

void Killfeed::update() {
	for (auto &i : feed) {
		i.timeLeft -= Time::deltaTime;
	}

	while (!feed.empty() && feed.front().timeLeft <= 0.0f) {
		feed.front().cleanup();
		feed.pop_front();
	}

	int yOffset = 70;

	for (auto &i : feed) {
		int w = i.from->rect.w + i.sym->rect.w + i.to->rect.w + 54;
		int xOffset = WINDOW_WIDTH - w;

		i.from->set_position(xOffset, yOffset);

		xOffset += i.from->rect.w + 10;
		i.sym->set_position(xOffset, yOffset - 2);

		xOffset += i.sym->rect.w + 10;
		i.to->set_position(xOffset, yOffset);

		yOffset += 35;
	}
}
