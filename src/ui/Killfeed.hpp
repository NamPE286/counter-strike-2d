#pragma once

#include <list>
#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"
#include "../ui/Text.hpp"

class Killfeed : public MonoBehaviour {
	struct Item {
		Text *from = nullptr, *tp = nullptr, *sym = nullptr;
	};
	std::list<Item> feed;

public:
	Killfeed(SDL_Renderer *renderer);

	void add(Player *from, Player *to, bool headshot);
	void render();
	void fixed_update();
};