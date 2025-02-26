#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"

class GameScene : public MonoBehaviour {
public:
	using MonoBehaviour::MonoBehaviour;
	void update();
};