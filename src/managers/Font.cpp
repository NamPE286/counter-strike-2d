#include "Font.hpp"

#include <stdexcept>
#include <string>

void Font::init() {
	if (TTF_Init() < 0) {
		throw std::runtime_error("Failed to initialize SDL_ttf. Error: " + std::string(TTF_GetError()));
	}
}

TTF_Font* Font::load(std::string filePath, int fontSize) {
	if (fontMap.contains({ filePath, fontSize })) {
		return fontMap[{ filePath, fontSize }];
	}

	return fontMap[{ filePath, fontSize }] = TTF_OpenFont(filePath.c_str(), fontSize);
}

void Font::loadBatch(std::vector<std::pair<std::string, int>> filePaths) {
	for (auto& i : filePaths) {
		load(i.first, i.second);
	}
}
