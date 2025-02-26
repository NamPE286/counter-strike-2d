#pragma once

class MonoBehaviour {
public:
	SDL_Renderer* const renderer = nullptr;

	MonoBehaviour(SDL_Renderer* const renderer);

	virtual void start() {};
	virtual void update() {};
	virtual void fixed_update() {};
	virtual void render() {};
};