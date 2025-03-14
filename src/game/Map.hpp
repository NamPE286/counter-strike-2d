#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

#include "../behaviours/MonoBehaviour.hpp"

class Map : public MonoBehaviour {
	tmx_map *map = nullptr;

public:
	Map(SDL_Renderer* renderer, std::string filePath);
	~Map();

	void render();
};