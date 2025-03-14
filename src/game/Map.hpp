#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <tmxlite/Map.hpp>

#include "../behaviours/MonoBehaviour.hpp"

class Map : public MonoBehaviour {
	tmx::Map map;

public:
	Map(SDL_Renderer* renderer, std::string filePath);

	void render();
};