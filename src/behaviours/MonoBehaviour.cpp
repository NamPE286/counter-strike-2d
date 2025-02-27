#include "MonoBehaviour.hpp"

#include <SDL2/SDL.h>

MonoBehaviour::MonoBehaviour(SDL_Renderer* renderer):
	renderer(renderer) 
{}

MonoBehaviour::~MonoBehaviour() {}

void MonoBehaviour::event_handler(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		on_key_down(event);
		break;
	case SDL_KEYUP:
		on_key_up(event);
		break;
	}
}