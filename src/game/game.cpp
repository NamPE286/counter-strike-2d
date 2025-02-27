#include "Game.hpp"

#include <stdexcept>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include "../managers/Time.hpp"
#include "../game_objects/Player.hpp"
#include "../common.h"
#include "../scenes/GameScene.hpp"

GameScene* scene = nullptr;

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw std::runtime_error("Failed to initialize SDL. SDL error: " + std::string(SDL_GetError()));
	}

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	window = SDL_CreateWindow("SDL", (DM.w - WINDOW_WIDTH) / 2, (DM.h - WINDOW_HEIGHT) / 2, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		throw std::runtime_error("Failed to create window. Error: " + std::string(SDL_GetError()));
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		throw std::runtime_error("Failed to create renderer. Error: " + std::string(SDL_GetError()));
	}

	scene = new GameScene(renderer);
}

Game::~Game() {
	delete scene;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::start_game_loop() {
	std::thread fixedUpdateThread([&](void){
		while (running) {
			auto start = std::chrono::high_resolution_clock().now();

			fixed_update();

			std::this_thread::sleep_for(std::chrono::milliseconds(FIXED_UPDATE_TIME_STEP));

			auto end = std::chrono::high_resolution_clock().now();
			Time::fixedDeltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(end - start).count();
		}
	});

	float frameTime = (float)1000 / MAX_FPS;

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			event_handler(event);
		}

		auto start = std::chrono::high_resolution_clock().now();

		update();
		render();

		auto end = std::chrono::high_resolution_clock().now();
		Time::deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(end - start).count();

		if (frameTime > Time::deltaTime) {
			SDL_Delay(Uint32(frameTime - Time::deltaTime));

			auto end = std::chrono::high_resolution_clock().now();
			Time::deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(end - start).count();
		}
	}

	fixedUpdateThread.join();
}

void Game::stop_game_loop() {
	running = false;
}

void Game::event_handler(SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		stop_game_loop();
	}

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			stop_game_loop();
		}
	}

	scene->event_handler(event);
}

void Game::update() {
	scene->update();
}

void Game::fixed_update() {
	scene->fixed_update();
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	scene->render();

	SDL_RenderPresent(renderer);
}