#include <SDL2/SDL.h>
#include "MonoBehaviour.hpp"

MonoBehaviour::MonoBehaviour(SDL_Renderer* renderer):
	renderer(renderer) 
{
	start();
}

void MonoBehaviour::start() {
}

void MonoBehaviour::update() {
}

void MonoBehaviour::fixed_update() {
}
