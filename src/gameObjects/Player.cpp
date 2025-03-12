#include "Player.hpp"

#include <iostream>

#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"
#include "../managers/Audio.hpp"

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

Player::Player(SDL_Renderer* renderer, int side, Vec2 pos,  bool playable):
	MonoBehaviour(renderer), position(pos), side(side), playable(playable)
{
	if (side == PlayerSide::T) {
		color = { 255, 205, 100, 255 };
	} else if (side == PlayerSide::CT) {
		color = { 154, 203, 249, 255 };
	}

	weapons[0] = new Weapon(renderer, "AK-47", &position, &velocity);
	weapons[1] = new Weapon(renderer, "Glock-18", &position, &velocity);
	weapons[2] = new Weapon(renderer, "Knife", &position, &velocity);

	if (playable) {
		weapons[weaponSlot]->equip();
	}
}

Player::~Player() {
	for (int i = 0; i < 3; i++) {
		delete weapons[i];
	}
}

void Player::update() {
	update_position();

	for (int i = 0; i < 3; i++) {
		weapons[i]->update();
	}
}

void Player::fixed_update() {
	footstepDelay -= Time::fixedDeltaTime;
	footstepDelay = std::max(footstepDelay, -1.0f);

	for (int i = 0; i < 3; i++) {
		weapons[i]->fixed_update();
	}

	if (velocity.magnitude() >= 0.35f && footstepDelay <= 0.0f) {
		footstepDelay = 360;
		Mix_PlayChannel(-1, Audio::load("assets/player/footsteps/concrete_ct_01.wav"), 0);
	}
}

void Player::render() {
	for (int i = 0; i < 3; i++) {
		weapons[i]->render();
	}

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

	SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void Player::on_key_down(SDL_Event& event) {
	const auto keyboard = SDL_GetKeyboardState(0);

	if (event.key.repeat) {
		return;
	}

	if (event.key.keysym.scancode == SDL_SCANCODE_1) {
		weapons[weaponSlot]->stop_firing();
		weapons[weaponSlot]->stop_reloading();

		weaponSlot = 0;
		weapons[weaponSlot]->equip();
	} else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
		weapons[weaponSlot]->stop_firing();
		weapons[weaponSlot]->stop_reloading();

		weaponSlot = 1;
		weapons[weaponSlot]->equip();
	} else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
		weapons[weaponSlot]->stop_firing();
		weapons[weaponSlot]->stop_reloading();

		weaponSlot = 2;
		weapons[weaponSlot]->equip();
	} else if (event.key.keysym.scancode == SDL_SCANCODE_R) {
		weapons[weaponSlot]->reload();
	} else if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
		maxSpeed /= 2;
	} else {
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
}

void Player::on_key_up(SDL_Event& event) {
	if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
		maxSpeed *= 2;
	} else {
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
}

void Player::on_mouse_button_down(SDL_Event& event) {
	weapons[weaponSlot]->fire(&position, &velocity);
}

void Player::on_mouse_button_up(SDL_Event& event) {
	weapons[weaponSlot]->stop_firing();
}

Weapon* Player::get_weapon() {
	return weapons[weaponSlot];
}
