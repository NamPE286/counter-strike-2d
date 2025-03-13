#include "MonoBehaviour.hpp"

#include <SDL2/SDL.h>
#include <stdexcept>

MonoBehaviour::MonoBehaviour(SDL_Renderer *renderer):
	renderer(renderer)
{
	if (renderer == nullptr) {
		throw std::runtime_error("Renderer cannot be null");
	}
}

MonoBehaviour::~MonoBehaviour() {}

void MonoBehaviour::event_handler(SDL_Event &event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		on_key_down(event);
		break;
	case SDL_KEYUP:
		on_key_up(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
		on_mouse_button_down(event);
		break;
	case SDL_MOUSEBUTTONUP:
		on_mouse_button_up(event);
		break;
	}
}