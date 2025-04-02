#include "GameScene.hpp"

#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <thread>
#include <vector>

#include "../game/Map.hpp"
#include "../managers/Audio.hpp"

GameScene::GameScene(SDL_Renderer *renderer):
	MonoBehaviour(renderer)
{
	match = new Match(renderer, "de_ancient");
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, match->map->w, match->map->h);

	if (!texture) {
		throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
	}

	match->add_player(new Player(
		renderer,
		this,
		"Me",
		PlayerSide::CT,
		Vec2(0, 0),
		true));
	match->add_player(new Player(
		renderer,
		this,
		"BOT A",
		PlayerSide::T,
		Vec2(0, 0),
		false));
	match->add_player(new Player(
		renderer,
		this,
		"BOT B",
		PlayerSide::T,
		Vec2(0, 0),
		false));

	self = match->players[0];
	hud = new HUD(renderer, self, match);
	camera = new PlayerCamera(renderer, 960, 540, texture, self);
	Audio::dest = &self->position;

	match->reset();
	ai = new PlayerAI(match, self);
}

GameScene::~GameScene() {
	delete hud;
	delete match;
	delete camera;
	delete ai;

	SDL_DestroyTexture(texture);
}

void GameScene::event_handler(SDL_Event &event) {
	if (match->phase != Phase::BUY || 
		(
			event.key.keysym.scancode != SDL_SCANCODE_W &&
			event.key.keysym.scancode != SDL_SCANCODE_A &&
			event.key.keysym.scancode != SDL_SCANCODE_S &&
			event.key.keysym.scancode != SDL_SCANCODE_D
		)
	) 
	{
		self->event_handler(event);
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
	for (Player *p : match->players) {
		p->update();
		match->map->collision_handler(p);
	}

	camera->update();
	hud->update();
	ai->update();

	auto *obj = match->map->get_callout(self->position.x, self->position.y);

	if (obj != nullptr) {
		hud->update_callout(obj->name);
	}

	if (match->update()) {
		if (match->winner == PlayerSide::T) {
			std::cout << "Terrorist win" << '\n';
		} else {
			std::cout << "Counter Terrorist win" << '\n';
		}

		std::thread t([&]() {
			if (match->winner == self->side) {
				Mix_PlayMusic(Audio::loadMusic("assets/sounds/music/wonround.mp3"), 0);
			} else {
				Mix_PlayMusic(Audio::loadMusic("assets/sounds/music/lostround.mp3"), 0);
			}

			SDL_Delay(1000);

			if (match->winner == PlayerSide::T) {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/terwin.wav"), 0);
			} else {
				Mix_PlayChannel(-1, Audio::loadWAV("assets/sounds/ctwin.wav"), 0);
			}
		});

		t.detach();
	}

	if (match->is_side_switched()) {
		delete hud;
		hud = new HUD(renderer, self, match);
	}
}

void GameScene::fixed_update() {
	for (Player *p : match->players) {
		p->fixed_update();
	}

	match->fixed_update();
}

void GameScene::render() {
	SDL_SetRenderTarget(renderer, texture);

	match->map->render();
	match->map->render_visible_area(camera->target, match->players, false);

	SDL_SetRenderTarget(renderer, nullptr);

	camera->render();
	hud->render();
}

void GameScene::on_mouse_button_down(SDL_Event &event) {
	if (match->phase != Phase::BUY) {
		self->fire();
	}
}

void GameScene::on_mouse_button_up(SDL_Event &event) {
	self->stop_firing();
}
