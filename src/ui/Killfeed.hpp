#pragma once

#include <list>
#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"
#include "../ui/Text.hpp"

class Killfeed : public MonoBehaviour {
	struct Item {
		Text *from = nullptr, *to = nullptr, *sym = nullptr;
		float timeLeft;
		bool isSelf;

		void cleanup() {
			delete from;
			delete to;
			delete sym;
		}
	};
	std::list<Item> feed;

public:
	Killfeed(SDL_Renderer *renderer);

	void add(Player *from, Player *to, bool headshot);
	void render();
	void update();
};