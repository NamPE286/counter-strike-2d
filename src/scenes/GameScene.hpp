#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "../ui/Text.hpp"
#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"

class GameScene : public MonoBehaviour {
	std::vector<Player> players;
	Text* text = nullptr;

public:
	GameScene(SDL_Renderer* renderer);
	void event_handler(SDL_Event& event);
	void update();
	void fixed_update();
	void render();
};