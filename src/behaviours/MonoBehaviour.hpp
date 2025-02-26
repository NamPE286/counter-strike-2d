#pragma once

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
};