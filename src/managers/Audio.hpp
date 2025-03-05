#pragma once

#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL_mixer.h>

namespace Audio {
	inline std::map<std::string, Mix_Chunk*> mp;

	void init();
	Mix_Chunk* load(std::string filePath);
	void load_batch(std::vector<std::string> filePaths);
	void destroy();
}