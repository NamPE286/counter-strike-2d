#include "GameScene.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "../managers/Font.hpp"
#include "../managers/Time.hpp"
#include "../common.h"

GameScene::GameScene(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	self = new Player(renderer, "Me", PlayerSide::T, Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	hud = new HUD(renderer, self);

	match.add_player(self);
	match.add_player(new Player(renderer, "BOT", PlayerSide::CT, Vec2(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2), false));
}

GameScene::~GameScene() {
	delete hud;
}

void GameScene::event_handler(SDL_Event &event) {
	for (Player *p : match.players) {
		if (!p->playable) {
			continue;
		}

		p->event_handler(event);
	}

	switch (event.type) {
	case SDL_KEYDOWN:
		on_key_down(event);
		break;
	case SDL_KEYUP:
		on_key_up(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
		on_mouse_button_down(event);
		break;
	case SDL_MOUSEBUTTONUP:
		on_mouse_button_up(event);
		break;
	}
}

void GameScene::update() {
	for (Player *p : match.players) {
		p->update();
	}

	hud->update();
	
	if (match.update()) {
		if (match.winner == Winner::T) {
			std::cout << "Terrorist win" << '\n';
		} else {
			std::cout << "Counter Terrorist win" << '\n';
		}
	}
}

void GameScene::fixed_update() {
	for (Player *p : match.players) {
		p->fixed_update();
	}

	match.fixed_update();
}

void GameScene::render() {
	for (Player *p : match.players) {
		p->render();
	}

	hud->render();
}

void GameScene::on_mouse_button_down(SDL_Event &event) {
	self->fire(&match.players);
}

void GameScene::on_mouse_button_up(SDL_Event &event) {
	self->stop_firing();
}
