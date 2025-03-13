#include "Font.hpp"

#include <stdexcept>
#include <string>

void Font::init() {
	if (TTF_Init() != 0) {
		throw std::runtime_error("Failed to initialize SDL_ttf. Error: " + std::string(TTF_GetError()));
	}
}

void Font::destroy() {
	for (auto &i : fontMap) {
		TTF_CloseFont(i.second);
	}

	TTF_Quit();
}

TTF_Font *Font::load(std::string filePath, int fontSize) {
	if (fontMap.contains({ filePath, fontSize })) {
		return fontMap[{ filePath, fontSize }];
	}

	TTF_Font *ptr = TTF_OpenFont(filePath.c_str(), fontSize);

	if (ptr == nullptr) {
		throw std::runtime_error("Failed to load font from " + filePath + ". Error: " + std::string(TTF_GetError()));
	}

	fontMap[{ filePath, fontSize }] = ptr;

	return ptr;
}

void Font::loadBatch(std::vector<std::pair<std::string, int>> filePaths) {
	for (auto &i : filePaths) {
		load(i.first, i.second);
	}
}
