#include "Player.hpp"

#include <iostream>

#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"

void Player::update_position() {
	velocity += acceleration * Time::deltaTime * 0.5;
	position += velocity * Time::deltaTime;
	velocity += acceleration * Time::deltaTime * 0.5;

	if (velocity.magnitude() > maxSpeed) {
		velocity = velocity.normalized() * maxSpeed;
	}

	if (velocity.x * direction.x <= 0) {
		velocity.x = acceleration.x = 0;
	}

	if (velocity.y * direction.y <= 0) {
		velocity.y = acceleration.y = 0;
	}
}

Player::Player(SDL_Renderer* renderer, int r, int g, int b, Vec2 pos):
	MonoBehaviour(renderer), position(pos), r(r), g(g), b(b)
{}

void Player::update() {
	update_position();
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

	if (keyboard[SDL_SCANCODE_A] && keyboard[SDL_SCANCODE_D]) {
		acceleration.x *= -1;
		return;
	}

	if (keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_S]) {
		acceleration.y *= -1;
		return;
	}

	if (directionMap[event.key.keysym.scancode].x != 0) {
		velocity.x = 0;
		direction.x = directionMap[event.key.keysym.scancode].x;
		acceleration.x = directionMap[event.key.keysym.scancode].x * 0.002f;
	}

	if (directionMap[event.key.keysym.scancode].y != 0) {
		velocity.y = 0;
		direction.y = directionMap[event.key.keysym.scancode].y;
		acceleration.y = directionMap[event.key.keysym.scancode].y * 0.002f;
	}
}

void Player::on_key_up(SDL_Event& event) {
	const auto keyboard = SDL_GetKeyboardState(0);

	if (directionMap[event.key.keysym.scancode].x != 0 && velocity.x != 0) {
		acceleration.x = directionMap[event.key.keysym.scancode].x * 0.002f * -1;
	}

	if (directionMap[event.key.keysym.scancode].y != 0 && velocity.y != 0) {
		acceleration.y = directionMap[event.key.keysym.scancode].y * 0.002f * -1;
	}

	if (keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S]) {
		velocity.y = 0;
		direction.y = directionMap[SDL_SCANCODE_W].y;
		acceleration.y = directionMap[SDL_SCANCODE_W].y * 0.002f;
	} else if (!keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_S]) {
		velocity.y = 0;
		direction.y = directionMap[SDL_SCANCODE_S].y;
		acceleration.y = directionMap[SDL_SCANCODE_S].y * 0.002f;
	}
	
	if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D]) {
		velocity.x = 0;
		direction.x = directionMap[SDL_SCANCODE_A].x;
		acceleration.x = directionMap[SDL_SCANCODE_A].x * 0.002f;
	} else if (!keyboard[SDL_SCANCODE_A] && keyboard[SDL_SCANCODE_D]) {
		velocity.x = 0;
		direction.x = directionMap[SDL_SCANCODE_D].x;
		acceleration.x = directionMap[SDL_SCANCODE_D].x * 0.002f;
	}
}

void Player::on_mouse_button_down(SDL_Event& event) {
	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);

	float angle = Utils::getAngle((int)position.x, (int)position.y, x, y);

	std::cout << angle * 180 / 3.14 << '\n';
}
