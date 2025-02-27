#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "../behaviours/MonoBehaviour.hpp"
#include "../game_objects/Player.hpp"

class GameScene : public MonoBehaviour {
	std::vector<Player> players;

public:
	GameScene(SDL_Renderer* renderer);
	void event_handler(SDL_Event& event);
	void update();
	void render();
};