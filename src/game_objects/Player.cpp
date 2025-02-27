#include "Player.hpp"

#include <iostream>

#include "../managers/Time.hpp"

Player::Player(SDL_Renderer* renderer, int r, int g, int b, Vec2 pos):
	MonoBehaviour(renderer), position(pos), r(r), g(g), b(b)
{}

void Player::update() {
	position += velocity * Time::deltaTime;
}

void Player::fixed_update() {}

void Player::render() {
	SDL_Rect rect = {
		(int)position.x - (size - borderWidth) / 2,
		(int)position.y - (size - borderWidth) / 2,
		size - borderWidth,
		size - borderWidth
	};
	SDL_Rect borderRect = {
		(int)position.x - size / 2,
		(int)position.y - size / 2,
		size,
		size
	};

	SDL_SetRenderDrawColor(renderer, r / 2, g / 2, b / 2, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void Player::on_key_down(SDL_Event& event) {
	const auto keyboard = SDL_GetKeyboardState(0);

	Vec2 direction = Vec2(0, 0);

	for (auto& [bind, v] : directionBind) {
		if (keyboard[bind]) {
			direction += v;
		}
	}

	velocity = direction * speed;
}

void Player::on_key_up(SDL_Event& event) {
	for (auto& [bind, v] : directionBind) {
		if (bind == event.key.keysym.scancode) {
			velocity += v * speed * -1;
			break;
		}
	}
}