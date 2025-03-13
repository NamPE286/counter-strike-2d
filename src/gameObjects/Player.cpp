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

void Player::change_weapon(int slot) {
	const auto keyboard = SDL_GetKeyboardState(0);

	weapons[weaponSlot]->stop_firing();
	weapons[weaponSlot]->stop_reloading();

	weaponSlot = slot;
	weapons[weaponSlot]->equip(playable);

	maxSpeed = 0.4f * weapons[weaponSlot]->mobility / 250;

	if (keyboard[SDL_SCANCODE_LSHIFT]) {
		maxSpeed /= 2;
	}
}

void Player::set_position(Vec2 newPos) {
	position = newPos;
}

void Player::take_damage(Weapon *w, bool headshot) {
	if (w->name == "Knife") {
		headshot = false;
	}

	velocity = velocity * w->taggingPower;

	float dmg = w->baseDamage;

	if (armor > 0) {
		dmg *= w->armorPenetration;
	}

	if (headshot) {
		dmg *= w->headshotMultiplier;
	}

	armor = std::max(0, armor - (int)(w->baseDamage * w->armorPenetration / 2));
	hp = std::max(0, hp - (int)dmg);

	if (hp == 0) {
		Mix_PlayChannel(-1, Audio::kill(), 0);
		Mix_PlayChannel(-1, Audio::death(), 0);

		if (headshot) {
			if (armor > 0 && helmet) {
				Mix_PlayChannel(-1, Audio::headshot_armor(), 0);
			} else {
				Mix_PlayChannel(-1, Audio::headshot_no_armor(), 0);
			}
		} else {
			if (armor > 0) {
				Mix_PlayChannel(-1, Audio::bodyshot_armor(), 0);
			} else {
				Mix_PlayChannel(-1, Audio::bodyshot_no_armor(), 0);
			}
		}

		armor = 0, helmet = false;
	} else {
		if (headshot) {
			if (armor > 0 && helmet) {
				Mix_PlayChannel(-1, Audio::headshot_armor_dink(), 0);
			} else {
				Mix_PlayChannel(-1, Audio::headshot_no_armor(), 0);
			}
		} else {
			if (armor > 0) {
				Mix_PlayChannel(-1, Audio::bodyshot_armor(), 0);
			} else {
				Mix_PlayChannel(-1, Audio::bodyshot_no_armor(), 0);
			}
		}
	}

	std::cout << "Damage taken: " << (int)dmg << ". HP: " << hp << " Armor: " << armor << " Helmet: " << helmet << '\n';
}

bool Player::collide(Bullet bullet) {
	SDL_Rect rect = {
		(int)position.x - size / 2,
		(int)position.y - size / 2,
		size,
		size
	};

	int endX = static_cast<int>(bullet.x + bullet.length * cos(bullet.angle));
	int endY = static_cast<int>(bullet.y + bullet.length * sin(bullet.angle));

	auto lines_intersect = [](int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		auto orientation = [](int x1, int y1, int x2, int y2, int x3, int y3) {
			int val = (y2 - y1) * (x3 - x2) - (x2 - x1) * (y3 - y2);

			if (val == 0) {
				return 0;
			}

			return (val > 0) ? 1 : 2;
			};

		int o1 = orientation(x1, y1, x2, y2, x3, y3);
		int o2 = orientation(x1, y1, x2, y2, x4, y4);
		int o3 = orientation(x3, y3, x4, y4, x1, y1);
		int o4 = orientation(x3, y3, x4, y4, x2, y2);

		if (o1 != o2 && o3 != o4) return true;

		return false;
		};

	if (lines_intersect(bullet.x, bullet.y, endX, endY, rect.x, rect.y, rect.x + rect.w, rect.y) ||
		lines_intersect(bullet.x, bullet.y, endX, endY, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h) ||
		lines_intersect(bullet.x, bullet.y, endX, endY, rect.x, rect.y, rect.x, rect.y + rect.h) ||
		lines_intersect(bullet.x, bullet.y, endX, endY, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h)) {
		return true;
	}

	return false;
}

bool Player::collide(int pointerX, int pointerY) {
	SDL_Rect rect = {
		(int)position.x - 15 / 2,
		(int)position.y - 15 / 2,
		15,
		15
	};

	return (pointerX >= rect.x && pointerX <= rect.x + rect.w &&
		pointerY >= rect.y && pointerY <= rect.y + rect.h);
}

Player::Player(SDL_Renderer *renderer, std::string name, int side, Vec2 pos, bool playable):
	MonoBehaviour(renderer), name(name), position(pos), side(side), playable(playable)
{
	if (side == PlayerSide::T) {
		color = { 255, 205, 100, 255 };
	} else if (side == PlayerSide::CT) {
		color = { 154, 203, 249, 255 };
	}

	weapons[0] = new Weapon(renderer, "AK-47", &position, &velocity, &pointerX, &pointerY);
	weapons[1] = new Weapon(renderer, "Glock-18", &position, &velocity, &pointerX, &pointerY);
	weapons[2] = new Weapon(renderer, "Knife", &position, &velocity, &pointerX, &pointerY);

	change_weapon(0);
}

Player::~Player() {
	for (int i = 0; i < 3; i++) {
		delete weapons[i];
	}
}

void Player::update() {
	update_position();

	Bullet bullet(renderer);

	while (weapons[weaponSlot]->poll_bullets(bullet)) {
		if (playerList == nullptr) {
			continue;
		}

		for (Player *p : *playerList) {
			if (p == this || p->hp == 0) {
				continue;
			}

			if (p->collide(bullet)) {
				p->take_damage(weapons[weaponSlot], p->collide(pointerX, pointerY));
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		weapons[i]->update();
	}
}

void Player::fixed_update() {
	SDL_GetMouseState(&pointerX, &pointerY);

	for (int i = 0; i < 3; i++) {
		weapons[i]->fixed_update();
	}

	if (velocity.magnitude() > 0 && !keyboard[SDL_SCANCODE_LSHIFT]) {
		if (footstepDelay <= 0.0f) {
			footstepDelay = 360;
			Mix_PlayChannel(-1, Audio::load("assets/player/footsteps/concrete_ct_01.wav"), 0);
		} else {
			footstepDelay -= Time::fixedDeltaTime;
			footstepDelay = std::max(footstepDelay, -1.0f);
		}
	} else if (!keyboard[SDL_SCANCODE_LSHIFT]) {
		footstepDelay = 360;
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

	if (hp > 0) {
		SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, 255);
		SDL_RenderFillRect(renderer, &borderRect);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Player::on_key_down(SDL_Event &event) {
	keyboard[event.key.keysym.scancode] = true;

	if (hp == 0) {
		return;
	}

	if (event.key.repeat) {
		return;
	}

	if (event.key.keysym.scancode == SDL_SCANCODE_1 && weaponSlot != 0) {
		change_weapon(0);
	} else if (event.key.keysym.scancode == SDL_SCANCODE_2 && weaponSlot != 1) {
		change_weapon(1);
	} else if (event.key.keysym.scancode == SDL_SCANCODE_3 && weaponSlot != 2) {
		change_weapon(2);
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

void Player::on_key_up(SDL_Event &event) {
	keyboard[event.key.keysym.scancode] = false;

	if (hp == 0) {
		return;
	}

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
			direction.y = directionMap[SDL_SCANCODE_W].y;
			acceleration.y = directionMap[SDL_SCANCODE_W].y * 0.002f;
		} else if (!keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_S]) {
			direction.y = directionMap[SDL_SCANCODE_S].y;
			acceleration.y = directionMap[SDL_SCANCODE_S].y * 0.002f;
		}

		if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D]) {
			direction.x = directionMap[SDL_SCANCODE_A].x;
			acceleration.x = directionMap[SDL_SCANCODE_A].x * 0.002f;
		} else if (!keyboard[SDL_SCANCODE_A] && keyboard[SDL_SCANCODE_D]) {
			direction.x = directionMap[SDL_SCANCODE_D].x;
			acceleration.x = directionMap[SDL_SCANCODE_D].x * 0.002f;
		}
	}
}

void Player::fire(std::vector<Player *> *players) {
	if (hp == 0) {
		return;
	}

	playerList = players;
	weapons[weaponSlot]->fire();
}

void Player::stop_firing() {
	if (hp == 0) {
		return;
	}

	weapons[weaponSlot]->stop_firing();
}

Weapon *Player::get_weapon() const {
	return weapons[weaponSlot];
}
