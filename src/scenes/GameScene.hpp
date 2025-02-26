#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"

class GameScene : public MonoBehaviour {
public:
	using MonoBehaviour::MonoBehaviour;

	void event_handler(SDL_Event& event);
	void update();
};