#include "GameScene.hpp"

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <thread>

#include "../managers/Font.hpp"
#include "../managers/Audio.hpp"
#include "../managers/Time.hpp"
#include "../utilities/Utils.hpp"
#include "../common.h"

GameScene::GameScene(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	map = new Map(renderer, "assets/tilemaps/de_ancient.tmx");
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, map->w, map->h);

	auto *TSpawn = map->get_spawn(PlayerSide::T);
	auto *CTSpawn = map->get_spawn(PlayerSide::CT);

	self = new Player(
		renderer,
		this,
		"Me",
		PlayerSide::T,
		Vec2(
			(float)Utils::getRandomRange((int)TSpawn->x, int(TSpawn->x + TSpawn->width)),
			(float)Utils::getRandomRange((int)TSpawn->y, int(TSpawn->y + TSpawn->height))),
		true);

	match.add_player(self);
	match.add_player(new Player(
		renderer,
		this,
		"BOT A",
		PlayerSide::CT,
		Vec2(
			(float)Utils::getRandomRange((int)CTSpawn->x, int(CTSpawn->x + CTSpawn->width)),
			(float)Utils::getRandomRange((int)CTSpawn->y, int(CTSpawn->y + CTSpawn->height))),
		false));

	hud = new HUD(renderer, self, &match);
	camera = new PlayerCamera(renderer, 960, 540, texture, self);

	if (!texture) {
		throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
	}

	Audio::dest = &self->position;
}

GameScene::~GameScene() {
	delete hud;
	delete map;
	delete camera;

	SDL_DestroyTexture(texture);
}

void GameScene::event_handler(SDL_Event &event) {
	if (match.phase != Phase::BUY || 
		(
			event.key.keysym.scancode != SDL_SCANCODE_W &&
			event.key.keysym.scancode != SDL_SCANCODE_A &&
			event.key.keysym.scancode != SDL_SCANCODE_S &&
			event.key.keysym.scancode != SDL_SCANCODE_D
		)
	) 
	{
		for (Player *p : match.players) {
			if (!p->playable) {
				continue;
			}

			p->event_handler(event);
		}
	}

	switch (event.type) {
	case SDL_KEYDOWN:
		on_key_down(event);
		break;
	case SDL_KEYUP:
		on_key_up(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
		on_mouse_button_down(event);
		break;
	case SDL_MOUSEBUTTONUP:
		on_mouse_button_up(event);
		break;
	}
}

void GameScene::update() {
	for (Player *p : match.players) {
		p->update();
		map->collision_handler(p);
	}

	camera->update();
	hud->update();

	auto *obj = map->get_callout(self->position.x, self->position.y);

	if (obj != nullptr) {
		hud->update_callout(obj->name);
	}

	if (match.update()) {
		if (match.winner == PlayerSide::T) {
			std::cout << "Terrorist win" << '\n';
		} else {
			std::cout << "Counter Terrorist win" << '\n';
		}

		std::thread t([&]() {
			if (match.winner == self->side) {
				Mix_PlayMusic(Audio::loadMusic("assets/sounds/music/wonround.mp3"), 0);
			} else {
				Mix_PlayMusic(Audio::loadMusic("assets/sounds/music/lostround.mp3"), 0);
			}

			SDL_Delay(1000);

			if (match.winner == PlayerSide::T) {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/terwin.wav"), 0);
			} else {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/ctwin.wav"), 0);
			}
		});

		t.detach();
	}
}

void GameScene::fixed_update() {
	for (Player *p : match.players) {
		p->fixed_update();
	}

	match.fixed_update();
}

void GameScene::render() {
	SDL_SetRenderTarget(renderer, texture);

	map->render();
	map->render_visible_area(self, match.players);

	SDL_SetRenderTarget(renderer, nullptr);

	camera->render();
	hud->render();
}

void GameScene::on_mouse_button_down(SDL_Event &event) {
	if (match.phase != Phase::BUY) {
		self->fire();
	}
}

void GameScene::on_mouse_button_up(SDL_Event &event) {
	self->stop_firing();
}
