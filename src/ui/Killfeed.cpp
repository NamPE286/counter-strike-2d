#include "Killfeed.hpp"

#include <iostream>

#include "../managers/Font.hpp"

Killfeed::Killfeed(SDL_Renderer *renderer):
	MonoBehaviour(renderer) 
{}

void Killfeed::add(Player *from, Player *to, bool headshot) {
	std::string sym{ from->get_weapon()->symbol };

	if (headshot) {
		sym += 'u';
	}

	Text symbolText(renderer, Font::load("assets/fonts/icon.ttf", 16), { 255, 255, 255, 255 });
	symbolText.set_content(sym);

	Text fromText(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), from->color);
	fromText.set_content(from->name);

	Text toText(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 16), to->color);
	toText.set_content(to->name);

	std::cout << from->name << " killed " << to->name << ' ' << (headshot ? "by headshot " : " ") << "with " << from->get_weapon()->name << '\n';
}

void Killfeed::render()
{}

void Killfeed::fixed_update()
{}
