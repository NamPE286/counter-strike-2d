#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../managers/Font.hpp"

class Text {
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	TTF_Font* font = nullptr;
	std::string content;
	SDL_Color color = { 0, 0, 0, 0 };
	SDL_Rect rect = { 0, 0, 0, 0 };

public:
	Text(SDL_Renderer* renderer, TTF_Font* font, std::string content, SDL_Color color);

	void update_content(std::string newContent);
	void render(int x, int y);
};