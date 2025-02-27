#include <iostream>
#include "GameScene.hpp"
#include "../managers/Time.hpp"

GameScene::GameScene(SDL_Renderer* renderer):
	MonoBehaviour(renderer)
{
	players.emplace_back(renderer, Vec2(10, 50));
}

void GameScene::event_handler(SDL_Event& event) {

}

void GameScene::update() {
}

void GameScene::render() {
	for (Player& p : players) {
		p.render();
	}
}
