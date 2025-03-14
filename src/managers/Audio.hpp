#pragma once

#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL_mixer.h>

namespace Audio {
	inline std::map<std::string, Mix_Chunk *> wavMp;
	inline std::map<std::string, Mix_Music *> mp3Mp;

	void init();
	void destroy();

	Mix_Chunk *loadWAV(std::string filePath);
	Mix_Music *loadMusic(std::string filePath);
	void load_batch(std::vector<std::string> filePaths);

	Mix_Chunk *death();
	Mix_Chunk *kill();
	Mix_Chunk *headshot_no_armor();
	Mix_Chunk *headshot_armor();
	Mix_Chunk *headshot_armor_dink();
	Mix_Chunk *bodyshot_no_armor();
	Mix_Chunk *bodyshot_armor();
}