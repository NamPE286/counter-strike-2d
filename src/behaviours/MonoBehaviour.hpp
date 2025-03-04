#pragma once

#include <SDL2/SDL.h>

class MonoBehaviour {
public:
	SDL_Renderer* const renderer = nullptr;

	MonoBehaviour(SDL_Renderer* const renderer);
	~MonoBehaviour();

	virtual void update() {};
	virtual void fixed_update() {};
	virtual void render() {};
	virtual void event_handler(SDL_Event& event);
	virtual void on_key_down(SDL_Event& event) {};
	virtual void on_key_up(SDL_Event& event) {};
	virtual void on_mouse_button_down(SDL_Event& event) {};
	virtual void on_mouse_button_up(SDL_Event& event) {};
};