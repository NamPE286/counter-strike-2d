#pragma once

#include <SDL2/SDL.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../cameras/PlayerCamera.hpp"
#include "../game/Match.hpp"
#include "../gameObjects/Player.hpp"
#include "../ui/HUD.hpp"

class GameScene : public MonoBehaviour {
	SDL_Texture *texture = nullptr;
	PlayerCamera *camera = nullptr;
	HUD *hud = nullptr;

public:
	Player *self = nullptr;
	Match *match = nullptr;

	GameScene(SDL_Renderer *renderer);
	~GameScene();

	void event_handler(SDL_Event &event);
	void update();
	void fixed_update();
	void render();
	void on_mouse_button_down(SDL_Event &event);
	void on_mouse_button_up(SDL_Event &event);
};