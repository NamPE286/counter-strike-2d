#include "Player.hpp"
#include "../scenes/GameScene.hpp"

#include <iostream>

#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"
#include "../managers/Audio.hpp"
#include "../common.h"

void Player::update_position() {
	prevPosition = position;
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

	maxSpeed = MAX_SPEED * weapons[weaponSlot]->mobility / 250;

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
		Audio::playWAV(Audio::death(), position);

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
		death++;
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

		armor = 0, helmet = false;
		death++;
	}

	std::cout << "Damage taken: " << (int)dmg << ". HP: " << hp << " Armor: " << armor << " Helmet: " << helmet << '\n';
}

void Player::reset() {
	for (Weapon *w : weapons) {
		w->reset();
	}

	velocity = acceleration = direction = Vec2(0, 0);
	hp = 100;
}

void Player::stop_movement() {
	velocity = acceleration = direction = Vec2(0, 0);
}

void Player::stop_movement_x() {
	velocity.x = acceleration.x = direction.x = 0;
}

void Player::stop_movement_y() {
	velocity.y = acceleration.y = direction.y = 0;
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

float Player::distance(Bullet bullet) {
	float endX = static_cast<float>(bullet.x + bullet.length * cos(bullet.angle));
	float endY = static_cast<float>(bullet.y + bullet.length * sin(bullet.angle));

	float A = position.x - static_cast<float>(bullet.x);
	float B = position.y - static_cast<float>(bullet.y);
	float C = endX - static_cast<float>(bullet.x);
	float D = endY - static_cast<float>(bullet.y);

	float dot = A * C + B * D;
	float len_sq = C * C + D * D;
	float param = (len_sq != 0) ? dot / len_sq : -1;

	float xx, yy;

	if (param < 0) {
		xx = static_cast<float>(bullet.x);
		yy = static_cast<float>(bullet.y);
	} else if (param > 1) {
		xx = endX;
		yy = endY;
	} else {
		xx = static_cast<float>(bullet.x) + param * C;
		yy = static_cast<float>(bullet.y) + param * D;
	}

	float dx = position.x - xx;
	float dy = position.y - yy;
	return sqrt(dx * dx + dy * dy);
}

Player::Player(SDL_Renderer *renderer, GameScene *scene, std::string name, int side, Vec2 pos, bool playable):
	MonoBehaviour(renderer), name(name), position(pos), side(side), playable(playable), target(scene)
{
	if (side == PlayerSide::T) {
		color = { 255, 205, 100, 255 };
	} else if (side == PlayerSide::CT) {
		color = { 154, 203, 249, 255 };
	}

	weapons[0] = new Weapon(renderer, "AK-47", this, scene);
	weapons[1] = new Weapon(renderer, "Glock-18", this, scene);
	weapons[2] = new Weapon(renderer, "Knife", this, scene);

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
		if (target == nullptr) {
			continue;
		}

		for (Player *p : target->match->players) {
			if (p == this || p->hp == 0) {
				continue;
			}

			if (p->collide(bullet)) {
				p->take_damage(weapons[weaponSlot], p->distance(bullet) < 3.7f);

				if (p->hp == 0) {
					money += weapons[weaponSlot]->killReward;
					money = std::min(money, 16000);
					kill++;
				}
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		weapons[i]->update();
	}
}

void Player::fixed_update() {
	for (int i = 0; i < 3; i++) {
		weapons[i]->fixed_update();
	}

	if (velocity.magnitude() > 0 && !keyboard[SDL_SCANCODE_LSHIFT]) {
		if (footstepDelay <= 0.0f) {
			footstepDelay = 360;
			Audio::playWAV(Audio::loadWAV("assets/player/footsteps/concrete_ct_01.wav"), position, -1, 1.5f);
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
			velocity.x = 0;
			acceleration.x *= -1;
			return;
		}

		if (keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_S]) {
			velocity.y = 0;
			acceleration.y *= -1;
			return;
		}

		if (directionMap[event.key.keysym.scancode].x != 0) {
			velocity.x = 0;
			direction.x = directionMap[event.key.keysym.scancode].x;
			acceleration.x = directionMap[event.key.keysym.scancode].x * MAX_ACCELERATION;
		}

		if (directionMap[event.key.keysym.scancode].y != 0) {
			velocity.y = 0;
			direction.y = directionMap[event.key.keysym.scancode].y;
			acceleration.y = directionMap[event.key.keysym.scancode].y * MAX_ACCELERATION;
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
	} else if (
		event.key.keysym.scancode == SDL_SCANCODE_W ||
		event.key.keysym.scancode == SDL_SCANCODE_S ||
		event.key.keysym.scancode == SDL_SCANCODE_A ||
		event.key.keysym.scancode == SDL_SCANCODE_D
		) {
		const auto keyboard = SDL_GetKeyboardState(0);

		if (directionMap[event.key.keysym.scancode].x != 0 && velocity.x != 0) {
			acceleration.x = directionMap[event.key.keysym.scancode].x * MAX_ACCELERATION * -1;
		}

		if (directionMap[event.key.keysym.scancode].y != 0 && velocity.y != 0) {
			acceleration.y = directionMap[event.key.keysym.scancode].y * MAX_ACCELERATION * -1;
		}

		if (keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S]) {
			direction.y = directionMap[SDL_SCANCODE_W].y;
			acceleration.y = directionMap[SDL_SCANCODE_W].y * MAX_ACCELERATION;
		} else if (!keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_S]) {
			direction.y = directionMap[SDL_SCANCODE_S].y;
			acceleration.y = directionMap[SDL_SCANCODE_S].y * MAX_ACCELERATION;
		}

		if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D]) {
			direction.x = directionMap[SDL_SCANCODE_A].x;
			acceleration.x = directionMap[SDL_SCANCODE_A].x * MAX_ACCELERATION;
		} else if (!keyboard[SDL_SCANCODE_A] && keyboard[SDL_SCANCODE_D]) {
			direction.x = directionMap[SDL_SCANCODE_D].x;
			acceleration.x = directionMap[SDL_SCANCODE_D].x * MAX_ACCELERATION;
		}
	}
}

void Player::fire() {
	if (hp == 0) {
		return;
	}

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
