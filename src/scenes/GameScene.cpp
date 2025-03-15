#include "GameScene.hpp"

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <thread>

#include "../managers/Font.hpp"
#include "../managers/Audio.hpp"
#include "../managers/Time.hpp"
#include "../common.h"

GameScene::GameScene(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	self = new Player(renderer, "Me", PlayerSide::T, Vec2(22 * 32, 23 * 32), true);
	hud = new HUD(renderer, self, &match);

	match.add_player(self);
	match.add_player(new Player(renderer, "BOT A", PlayerSide::CT, Vec2(30 * 32, 32 * 32), false));

	map = new Map(renderer, "assets/tilemaps/test.tmx");
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, map->w, map->h);
	camera = new PlayerCamera(renderer, 960, 540, texture, self);

	if (!texture) {
		throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
	}
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
	}

	camera->update();
	map->collision_handler(self);
	hud->update();
	
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
	SDL_SetRenderDrawColor(renderer, 137, 137, 137, 255);
	SDL_RenderClear(renderer);

	map->render();

	for (Player *p : match.players) {
		p->render();
	}

	SDL_SetRenderTarget(renderer, nullptr);

	camera->render();
	hud->render();
}

void GameScene::on_mouse_button_down(SDL_Event &event) {
	if (match.phase != Phase::BUY) {
		self->fire(this);
	}
}

void GameScene::on_mouse_button_up(SDL_Event &event) {
	self->stop_firing();
}
