#pragma once

#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"

class PlayerCamera : public MonoBehaviour {
public:
	void render();
};