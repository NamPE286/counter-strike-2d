#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../managers/Font.hpp"

class Text {
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	TTF_Font* font = nullptr;
	SDL_Color color = { 0, 0, 0, 0 };
	SDL_Rect rect = { 0, 0, 0, 0 };
	std::string content;

public:
	Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);
	~Text();

	void set_content(std::string newContent);
	void set_position(int x, int y);
	void render();
};