#include "MiniScoreboard.hpp"

#include "../managers/Font.hpp"
#include "../utilities/Utils.hpp"
#include "../common.h"

MiniScoreboard::MiniScoreboard(SDL_Renderer *renderer, Match *match):
	MonoBehaviour(renderer), match(match)
{
	timerText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 23), { 255, 255, 255, 255 });
	TScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 23), { 255, 205, 100, 255 });
	CTScoreText = new Text(renderer, Font::load("assets/fonts/stratum2-bold.ttf", 23), { 154, 203, 249, 255 });
	TAliveText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 38), { 255, 205, 100, 255 });
	CTAliveText = new Text(renderer, Font::load("assets/fonts/stratum2-medium.ttf", 38), { 154, 203, 249, 255 });

	aliveT = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 14), { 255, 205, 100, 255 });
	aliveCT = new Text(renderer, Font::load("assets/fonts/stratum2-regular.ttf", 14), { 154, 203, 249, 255 });
	
	aliveT->set_content("Alive");
	aliveCT->set_content("Alive");

	aliveT->set_position(TAliveRect.x + (TAliveRect.w - aliveT->rect.w) / 2, TAliveRect.y + (TAliveRect.h - aliveT->rect.h) / 2 + 17);
	aliveCT->set_position(CTAliveRect.x + (CTAliveRect.w - aliveCT->rect.w) / 2, CTAliveRect.y + (CTAliveRect.h - aliveCT->rect.h) / 2 + 17);
}

void MiniScoreboard::update() {
	int m = static_cast<int>(match->timeLeft) / 60;
	int s = static_cast<int>(match->timeLeft) % 60;

	if (match->phase == Phase::WARMUP) {
		timerText->set_content("");
	} else {
		timerText->set_content(Utils::getClockString(m, s));
		timerText->set_position(timerRect.x + (timerRect.w - timerText->rect.w) / 2, timerRect.y + (timerRect.h - timerText->rect.h) / 2);
	}

	TScoreText->set_content(std::to_string(match->scores.first));
	TScoreText->set_position(TScoreRect.x + (TScoreRect.w - TScoreText->rect.w) / 2, TScoreRect.y + (TScoreRect.h - TScoreText->rect.h) / 2);

	CTScoreText->set_content(std::to_string(match->scores.second));
	CTScoreText->set_position(CTScoreRect.x + (CTScoreRect.w - CTScoreText->rect.w) / 2, CTScoreRect.y + (CTScoreRect.h - CTScoreText->rect.h) / 2);

	TAliveText->set_content(std::to_string(match->alive.first));
	TAliveText->set_position(TAliveRect.x + (TAliveRect.w - TAliveText->rect.w) / 2, TAliveRect.y + (TAliveRect.h - TAliveText->rect.h) / 2 - 5);

	CTAliveText->set_content(std::to_string(match->alive.second));
	CTAliveText->set_position(CTAliveRect.x + (CTAliveRect.w - CTAliveText->rect.w) / 2, CTAliveRect.y + (CTAliveRect.h - CTAliveText->rect.h) / 2 - 5);
}

void MiniScoreboard::render() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 150);

	SDL_RenderFillRect(renderer, &timerRect);
	SDL_RenderFillRect(renderer, &TScoreRect);
	SDL_RenderFillRect(renderer, &CTScoreRect);
	SDL_RenderFillRect(renderer, &TAliveRect);
	SDL_RenderFillRect(renderer, &CTAliveRect);

	timerText->render();
	TScoreText->render();
	CTScoreText->render();
	TAliveText->render();
	CTAliveText->render();

	aliveT->render();
	aliveCT->render();
}
