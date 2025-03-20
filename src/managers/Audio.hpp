#pragma once

#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL_mixer.h>

#include "../geometry/Vec2.hpp"

namespace Audio {
	inline std::map<std::string, Mix_Chunk *> wavMp;
	inline std::map<std::string, Mix_Music *> mp3Mp;
	inline Vec2 *dest = nullptr;

	void init();
	void destroy();

	Mix_Chunk *loadWAV(std::string filePath);
	Mix_Music *loadMusic(std::string filePath);
	void load_batch(std::vector<std::string> filePaths);
	void playWAV(Mix_Chunk *chunk, Vec2 src, int channel = -1, float multiplier = 1.0f);

	Mix_Chunk *death();
	Mix_Chunk *kill();
	Mix_Chunk *headshot_no_armor();
	Mix_Chunk *headshot_armor();
	Mix_Chunk *headshot_armor_dink();
	Mix_Chunk *bodyshot_no_armor();
	Mix_Chunk *bodyshot_armor();
}