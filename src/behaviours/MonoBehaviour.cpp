#include "MonoBehaviour.hpp"

#include <SDL2/SDL.h>

MonoBehaviour::MonoBehaviour(SDL_Renderer* renderer):
	renderer(renderer) 
{}

MonoBehaviour::~MonoBehaviour() {}