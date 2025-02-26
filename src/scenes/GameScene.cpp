#include "GameScene.hpp"
#include "../game/Time.hpp"
#include <iostream>

void GameScene::update() {
	std::cout << Time::deltaTime << '\n';
}