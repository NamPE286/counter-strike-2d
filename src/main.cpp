#include <SDL2/SDL.h>
#include "game/Game.hpp"

int SDL_main(int argc, char* argv[]) {
	Game game;
	
	game.start_game_loop();

	return 0;
}