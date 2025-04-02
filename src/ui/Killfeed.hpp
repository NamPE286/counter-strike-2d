#pragma once

#include <list>
#include <string>

#include "../behaviours/MonoBehaviour.hpp"
#include "../ui/Text.hpp"

class Killfeed : public MonoBehaviour {
	struct Item {
		std::string from, to;
		Text *gun = nullptr;
	};
	std::list<Item> feed;

public:
	void render();
	void fixed_update();
};