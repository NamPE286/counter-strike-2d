#include "GameScene.hpp"
#include "../managers/Time.hpp"
#include <iostream>

void GameScene::event_handler(SDL_Event& event) {

}

void GameScene::update() {
	std::cout << Time::deltaTime << ' ' << Time::fixedDeltaTime << '\n';
}

void GameScene::render() {
	SDL_Rect rect = { 100, 100, 100, 100 };

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}
