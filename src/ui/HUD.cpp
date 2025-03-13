#include "HUD.hpp"

#include <string>
#include <map>
#include "../common.h"

HUD::HUD(SDL_Renderer *renderer, Player *player):
	MonoBehaviour(renderer), player(player)
{
	if (player->side == PlayerSide::T) {
		color = { 255, 205, 100, 255 };
	} else if (player->side == PlayerSide::CT) {
		color = { 154, 203, 249, 255 };
	}

	ammoText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 40), color);
	ammoText->set_position(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT - 60);

	reserveAmmoText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 20), color);
	reserveAmmoText->set_position(WINDOW_WIDTH / 2 + 268, WINDOW_HEIGHT - 50);

	moneyText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 30), color);
	moneyText->set_position(15, WINDOW_HEIGHT - 50);

	hpText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 32), color);
	hpText->set_position(WINDOW_WIDTH / 2 - 268, WINDOW_HEIGHT - 53);

	armorText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 18), color);
	armorText->set_position(WINDOW_WIDTH / 2 - 320, WINDOW_HEIGHT - 43);

	primaryGun = new Text(renderer, Font::load("assets/fonts/icon.ttf", 100), color);
	primaryGun->set_position(WINDOW_WIDTH - 140, WINDOW_HEIGHT - 300);

	primaryGunBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	primaryGunBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 280);
	primaryGunBindText->set_content("1");

	secondaryGun = new Text(renderer, Font::load("assets/fonts/icon.ttf", 50), color);
	secondaryGun->set_position(WINDOW_WIDTH - 90, WINDOW_HEIGHT - 200);

	secondaryGunBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	secondaryGunBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 205);
	secondaryGunBindText->set_content("2");

	knife = new Text(renderer, Font::load("assets/fonts/icon.ttf", 100), color);
	knife->set_position(WINDOW_WIDTH - 140, WINDOW_HEIGHT - 150);
	knife->set_content("M");

	knifeBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	knifeBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 130);
	knifeBindText->set_content("3");

	weaponNameText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });

	scoreboard = new Scoreboard(renderer);
}

HUD::~HUD() {
	delete ammoText;
	delete reserveAmmoText;
	delete moneyText;
	delete hpText;
	delete armorText;
	delete primaryGun;
	delete secondaryGun;
	delete knife;
	delete weaponNameText;
	delete primaryGunBindText;
	delete secondaryGunBindText;
	delete knifeBindText;
	delete scoreboard;
}

void HUD::update() {
	std::map<std::string, std::string> mp = {
		{"AK-47", "A"},
		{"Glock-18", "K"}
	};

	ammoText->set_content(std::to_string(player->get_weapon()->ammo));
	reserveAmmoText->set_content(std::to_string(player->get_weapon()->reserveAmmo));
	moneyText->set_content("$" + std::to_string(player->money));
	hpText->set_content(std::to_string(player->hp));
	armorText->set_content(std::to_string(player->armor));

	if (player->weapons[0] != nullptr) {
		primaryGun->set_content(mp[player->weapons[0]->name]);
	} else {
		primaryGun->set_content("");
	}

	if (player->weapons[0] != nullptr) {
		secondaryGun->set_content(mp[player->weapons[1]->name]);
	} else {
		secondaryGun->set_content("");
	}

	if (player->weapons[player->weaponSlot] != nullptr) {
		weaponNameText->set_content(player->weapons[player->weaponSlot]->name);
		weaponNameText->set_position(WINDOW_WIDTH - weaponNameText->rect.w - 40, WINDOW_HEIGHT - 70 - 80 * (2 - player->weaponSlot));
	} else {
		weaponNameText->set_content("");
	}

	healthBarRect.w = 43 * player->hp / 100;
}

void HUD::render() {
	const auto keyboard = SDL_GetKeyboardState(0);

	if (player->hp == 0) {
		return;
	}

	SDL_Rect ammoSep = { ammoText->rect.x + 58, ammoText->rect.y + 12, 2, 21 };
	SDL_Rect helmetRect = { WINDOW_WIDTH / 2 - 311, WINDOW_HEIGHT - 48, 7, 5 };

	if (player->get_weapon()->ammo != -1) {
		ammoText->render();
		reserveAmmoText->render();

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
		SDL_RenderFillRect(renderer, &ammoSep);
	}

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(renderer, &healthBarRect);

	moneyText->render();
	hpText->render();

	primaryGun->render();
	secondaryGun->render();
	knife->render();

	primaryGunBindText->render();
	secondaryGunBindText->render();
	knifeBindText->render();

	weaponNameText->render();

	if (player->armor > 0) {
		armorText->render();

		if (player->helmet) {
			SDL_RenderFillRect(renderer, &helmetRect);
		}
	}

	if (keyboard[SDL_SCANCODE_TAB]) {
		scoreboard->render();
	}
}
