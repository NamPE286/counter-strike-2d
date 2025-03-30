#pragma once

#include <map>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <utility>
#include <vector>

namespace Font {
	inline std::map<std::pair<std::string, int>, TTF_Font *> fontMap;

	void init();
	void destroy();
	TTF_Font *load(std::string filePath, int fontSize);
	void loadBatch(std::vector<std::pair<std::string, int>> filePaths);
}