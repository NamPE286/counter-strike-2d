#pragma once

class MonoBehaviour {
	SDL_Renderer* const renderer = nullptr;

	void start();
	void update();
	void fixed_update();

public:
	MonoBehaviour(SDL_Renderer* const renderer);
};