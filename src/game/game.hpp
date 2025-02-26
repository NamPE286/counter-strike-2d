#pragma once

#include <SDL2/SDL.h>

class Game {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool running = true;

public:
	Game();
	~Game();

	void start_game_loop();
	void stop_game_loop();
	void event_handler(SDL_Event& event);

	// Call all update method. This method is called every frame
	void update();

	// Call all fixed_update method. This method is called every FIXED_UPDATE_TIME_STEP
	void fixed_update();

	// Call all render method. This method is called after all update finished
	void render();
};