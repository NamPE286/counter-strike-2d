#include <SDL2/SDL.h>
#include "MonoBehaviour.hpp"

MonoBehaviour::MonoBehaviour(SDL_Renderer* renderer):
	renderer(renderer) 
{}

MonoBehaviour::~MonoBehaviour() {}