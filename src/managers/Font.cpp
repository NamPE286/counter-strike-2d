#include "Font.hpp"

#include <stdexcept>
#include <string>

void Font::init() {
	if (TTF_Init() == -1) {
		throw std::runtime_error("Failed to initialize SDL_ttf. Error: " + std::string(TTF_GetError()));
	}
}

TTF_Font* Font::load(std::string filePath) {
	
}

void Font::loadBatch(std::vector<std::string> filePaths) {
}
