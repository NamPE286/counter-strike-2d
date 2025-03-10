#include "GameScene.hpp"

#include <iostream>
#include <string>

#include "../managers/Font.hpp"
#include "../managers/Time.hpp"
#include "../common.h"

GameScene::GameScene(SDL_Renderer* renderer):
	MonoBehaviour(renderer)
{
	players.emplace_back(renderer, 0, 255, 0, Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	hud = new HUD(renderer, &players[0], { 255, 205, 100, 255 });
}

void GameScene::event_handler(SDL_Event& event) {
	for (Player& p : players) {
		if (!p.playable) {
			continue;
		}

		p.event_handler(event);
	}
}

void GameScene::update() {
	for (Player& p : players) {
		p.update();
	}

	hud->update();
}

void GameScene::fixed_update() {
	for (Player& p : players) {
		p.fixed_update();
	}
}

void GameScene::render() {
	for (Player& p : players) {
		p.render();
	}
	

	hud->render();
}
