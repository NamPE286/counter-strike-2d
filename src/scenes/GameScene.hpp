#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"

class GameScene : private MonoBehaviour {
public:
	using MonoBehaviour::MonoBehaviour;
};