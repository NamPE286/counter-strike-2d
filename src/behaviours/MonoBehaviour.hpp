#pragma once

#include <SDL2/SDL.h>

class MonoBehaviour {
public:
	SDL_Renderer* const renderer = nullptr;

	MonoBehaviour(SDL_Renderer* const renderer);
	~MonoBehaviour();

	// Called at the end of destructor
	virtual void clean_up() {};
	virtual void start() {};
	virtual void update() {};
	virtual void fixed_update() {};
	virtual void render() {};
	virtual void event_handler(SDL_Event& event) {};
};