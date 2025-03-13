#include "Mouse.hpp"

Mouse::Mouse(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	SDL_ShowCursor(0);
}

void Mouse::update() {
	SDL_GetMouseState(&x, &y);
}

void Mouse::render() {
	SDL_Rect arr[4] = {
		{x - h / 2 - gap / 2, y - w / 2, h / 2, w},
		{x + gap / 2, y - w / 2, h / 2, w},
		{x - w / 2, y - h / 2 - gap / 2, w, h / 2},
		{x - w / 2, y + gap / 2, w, h / 2}
	};

	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	for (int i = 0; i < 4; i++) {
		SDL_RenderFillRect(renderer, &arr[i]);
	}
}
