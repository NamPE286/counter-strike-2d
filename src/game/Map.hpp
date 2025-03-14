#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <tmx.h>

#include "../behaviours/MonoBehaviour.hpp"

class Map : public MonoBehaviour {

public:
	Map(SDL_Renderer* renderer, std::string filePath);

	void render();
};