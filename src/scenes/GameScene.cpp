#include "GameScene.hpp"

#include <iostream>
#include <string>

#include "../managers/Font.hpp"
#include "../managers/Time.hpp"
#include "../common.h"

GameScene::GameScene(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	players.push_back(new Player(renderer, "Me", PlayerSide::T, Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)));
	players.push_back(new Player(renderer, "BOT", PlayerSide::CT, Vec2(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2), false));

	self = players[0];
	hud = new HUD(renderer, self);
}

GameScene::~GameScene() {
	for (Player *p : players) {
		delete p;
	}
}

void GameScene::event_handler(SDL_Event &event) {
	for (Player *p : players) {
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
	for (Player *p : players) {
		p->update();
	}

	hud->update();
}

void GameScene::fixed_update() {
	for (Player *p : players) {
		p->fixed_update();
	}
}

void GameScene::render() {
	for (Player *p : players) {
		p->render();
	}

	hud->render();
}

void GameScene::on_mouse_button_down(SDL_Event &event) {
	self->fire(&players);
}

void GameScene::on_mouse_button_up(SDL_Event &event) {
	self->stop_firing();
}
