#include "HUD.hpp"

#include <string>
#include "../common.h"

HUD::HUD(SDL_Renderer* renderer, Player* player, SDL_Color color):
	MonoBehaviour(renderer), player(player), color(color)
{
	ammoText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 40), color);
	ammoText->set_position(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT - 80);

	reserveAmmoText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 20), color);
	reserveAmmoText->set_position(WINDOW_WIDTH / 2 + 268, WINDOW_HEIGHT - 70);
}

void HUD::update() {
	ammoText->set_content(std::to_string(player->get_weapon()->ammo));
	reserveAmmoText->set_content(std::to_string(player->get_weapon()->reserveAmmo));
}

void HUD::render() {
	SDL_Rect ammoSep = {ammoText->rect.x + 58, ammoText->rect.y + 12, 2, 21};

	ammoText->render();
	reserveAmmoText->render();

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
	SDL_RenderFillRect(renderer, &ammoSep);
}
