#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <utility>
#include <map>

namespace Font {
	inline std::map<std::string, TTF_Font*> fontMap;

	void init();
	TTF_Font* load(std::string filePath);
	void loadBatch(std::vector<std::string> filePaths);
}