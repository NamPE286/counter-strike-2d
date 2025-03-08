#include "Scene.hpp"

void Scene::update() {
	if (stack.empty()) {
		return;
	}

	stack.top()->update();
}

void Scene::fixed_update() {
	if (stack.empty()) {
		return;
	}

	stack.top()->fixed_update();
}

void Scene::render() {
	if (stack.empty()) {
		return;
	}

	stack.top()->render();
}

void Scene::event_handler(SDL_Event& event) {
	if (stack.empty()) {
		return;
	}

	stack.top()->event_handler(event);
}


