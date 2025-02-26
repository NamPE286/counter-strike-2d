#include <stdexcept>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "Game.hpp"
#include "Time.hpp"
#include "../common.h"

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
}

void Game::start_game_loop() {
	std::thread fixedUpdateThread([&](void){
		while (running) {
			auto start = std::chrono::high_resolution_clock().now();

			fixed_update();

			auto end = std::chrono::high_resolution_clock().now();
			Time::fixedDeltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(start - end).count();

			std::this_thread::sleep_for(std::chrono::milliseconds(FIXED_UPDATE_TIME_STEP));
		}
	});

	float deltaTime = 1.0f;
	float frameTime = (float)1000 / MAX_FPS;

	while (running) {
		SDL_Event event;
		auto start = std::chrono::high_resolution_clock().now();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				stop_game_loop();
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					stop_game_loop();
				}
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			update();
			render();

			auto end = std::chrono::high_resolution_clock().now();
			Time::deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(end - start).count();

			if (frameTime > deltaTime) {
				SDL_Delay(Uint32(frameTime - deltaTime));
			}
		}
	}

	fixedUpdateThread.join();
}

void Game::stop_game_loop() {
	running = false;
}

void Game::update() {

}

void Game::fixed_update() {
	
}

void Game::render() {
	SDL_RenderPresent(renderer);
}