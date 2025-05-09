#include "HUD.hpp"

#include <cmath>
#include <map>
#include <string>

#include "../common.h"
#include "../managers/Font.hpp"
#include "../utilities/Utils.hpp"

HUD::HUD(SDL_Renderer *renderer, Player *player, Match *match):
	MonoBehaviour(renderer), player(player), match(match)
{
	if (player->side == PlayerSide::T) {
		color = TColor;
	} else if (player->side == PlayerSide::CT) {
		color = CTColor;
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

	primaryGun = new Text(renderer, Font::load("assets/fonts/icon.ttf", 40), color);
	secondaryGun = new Text(renderer, Font::load("assets/fonts/icon.ttf", 40), color);
	knife = new Text(renderer, Font::load("assets/fonts/icon.ttf", 40), color);

	primaryGunBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	primaryGunBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 280);
	primaryGunBindText->set_content("1");

	secondaryGunBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	secondaryGunBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 205);
	secondaryGunBindText->set_content("2");

	knifeBindText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });
	knifeBindText->set_position(WINDOW_WIDTH - 25, WINDOW_HEIGHT - 130);
	knifeBindText->set_content("3");

	weaponNameText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 14), { 255, 255, 255, 255 });

	calloutText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 20), color);
	calloutText->set_position(10, 10);

	scoreboard = new Scoreboard(renderer, match);
	miniScoreboard = new MiniScoreboard(renderer, match);
	alert = new Alert(renderer, color);
	announcer = new Announcer(renderer, color);
	killfeed = new Killfeed(renderer);
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
	delete alert;
}

void HUD::update() {
	ammoText->set_content(std::to_string(player->get_weapon()->ammo));
	reserveAmmoText->set_content(std::to_string(player->get_weapon()->reserveAmmo));
	moneyText->set_content("$" + std::to_string(player->money));
	hpText->set_content(std::to_string(player->hp));
	armorText->set_content(std::to_string(player->armor));

	if (player->weapons[0] != nullptr) {
		primaryGun->set_content(std::string{ player->weapons[0]->symbol });
		primaryGun->set_position(WINDOW_WIDTH - primaryGun->rect.w - 30, primaryGunBindText->rect.y + 10);
	} else {
		primaryGun->set_content("");
	}

	if (player->weapons[1] != nullptr) {
		secondaryGun->set_content(std::string{ player->weapons[1]->symbol });
		secondaryGun->set_position(WINDOW_WIDTH - secondaryGun->rect.w - 30, secondaryGunBindText->rect.y + 10);
	} else {
		secondaryGun->set_content("");
	}

	if (player->weapons[2] != nullptr) {
		knife->set_content(std::string{ player->weapons[2]->symbol });
		knife->set_position(WINDOW_WIDTH - knife->rect.w - 30, knifeBindText->rect.y + 10);
	} else {
		knife->set_content("");
	}

	if (player->weapons[player->weaponSlot] != nullptr) {
		weaponNameText->set_content(player->weapons[player->weaponSlot]->name);
		weaponNameText->set_position(WINDOW_WIDTH - weaponNameText->rect.w - 40, WINDOW_HEIGHT - 70 - 80 * (2 - player->weaponSlot));
	} else {
		weaponNameText->set_content("");
	}

	healthBarRect.w = 43 * player->hp / 100;
	scoreboard->update();
	miniScoreboard->update();

	if (match->phase == Phase::WARMUP) {
		int m = (int)std::ceil(match->timeLeft) / 60;
		int s = (int)match->timeLeft % 60;

		alert->set_content("Warmup " + Utils::getClockString(m, s));
	} else if (match->is_match_point_alert_visible()) {
		alert->set_content("MATCH POINT");
	} else if (match->is_match_start_alert_visible()) {
		alert->set_content("MATCH STARTED");
	}  else if (match->is_last_round_half_alert_visible()) {
		alert->set_content("LAST ROUND OF FIRST HALF");
	} else {
		alert->set_content("");
	}

	scoreboard->update();
	killfeed->update();
}

void HUD::fixed_update() {
}

void HUD::update_callout(std::string s) {
	calloutText->set_content(s);
}

void HUD::render() {
	const auto keyboard = SDL_GetKeyboardState(0);

	miniScoreboard->render();
	alert->render();
	killfeed->render();

	if (keyboard[SDL_SCANCODE_TAB]) {
		scoreboard->render();
	}

	if (match->phase == Phase::POST_ROUND) {
		announcer->render(player->side == match->winner);
	}

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

	primaryGun->render();
	secondaryGun->render();
	knife->render();

	moneyText->render();
	hpText->render();
	primaryGunBindText->render();
	secondaryGunBindText->render();
	knifeBindText->render();
	weaponNameText->render();
	calloutText->render();

	if (player->armor > 0) {
		armorText->render();

		if (player->helmet) {
			SDL_RenderFillRect(renderer, &helmetRect);
		}
	}
}