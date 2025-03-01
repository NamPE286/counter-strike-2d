#include "Player.hpp"

#include <iostream>

#include "../managers/Time.hpp"

Player::Player(SDL_Renderer* renderer, int r, int g, int b, Vec2 pos):
	MonoBehaviour(renderer), position(pos), r(r), g(g), b(b)
{}

void Player::update() {
	velocity += acceleration * Time::deltaTime;

	if (velocity.magnitude() > maxSpeed) {
		velocity = velocity.normalized() * maxSpeed;
	}

	if (velocity.x * direction.x < 0) {
		velocity.x = acceleration.x = direction.x = 0;
	}

	if (velocity.y * direction.y < 0) {
		velocity.y = acceleration.y = direction.y = 0;
	}

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

	if (event.key.repeat) {
		return;
	}

	if (directionMap[event.key.keysym.sym].x != 0) {
		velocity.x = 0;
		direction.x = directionMap[event.key.keysym.sym].x;
		acceleration.x = directionMap[event.key.keysym.sym].x * 0.002f;
	}

	if (directionMap[event.key.keysym.sym].y != 0) {
		velocity.y = 0;
		direction.y = directionMap[event.key.keysym.sym].y;
		acceleration.y = directionMap[event.key.keysym.sym].y * 0.002f;
	}
}

void Player::on_key_up(SDL_Event& event) {
	if (directionMap[event.key.keysym.sym].x != 0) {
		acceleration.x = directionMap[event.key.keysym.sym].x * 0.002f * -1;
	}

	if (directionMap[event.key.keysym.sym].y != 0) {
		acceleration.y = directionMap[event.key.keysym.sym].y * 0.002f * -1;
	}
}