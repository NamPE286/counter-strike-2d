#include "HUD.hpp"

#include <string>
#include "../common.h"

HUD::HUD(SDL_Renderer* renderer, Player* player):
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
}

void HUD::update() {
	ammoText->set_content(std::to_string(player->get_weapon()->ammo));
	reserveAmmoText->set_content(std::to_string(player->get_weapon()->reserveAmmo));
	moneyText->set_content("$" + std::to_string(player->money));
	hpText->set_content(std::to_string(player->hp));
	armorText->set_content(std::to_string(player->armor));

	healthBarRect.w = 43 * player->hp / 100;
}

void HUD::render() {
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

	if (player->armor > 0) {
		armorText->render();

		if (player->helmet) {
			SDL_RenderFillRect(renderer, &helmetRect);
		}
	}
}
