#include "Audio.hpp"

#include <iostream>
#include <stdexcept>

#include "../utilities/Utils.hpp"

void Audio::init() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error("Failed to initialize SDL_mixer. Error: " + std::string(Mix_GetError()));
	}
}

void Audio::destroy() {
	for (auto& i : mp) {
		Mix_FreeChunk(i.second);
	}

	Mix_Quit();
}

Mix_Chunk* Audio::load(std::string filePath) {
	if (mp.contains(filePath)) {
		return mp[filePath];
	}

	std::cout << "Loading " + filePath << '\n';

	auto* ptr = Mix_LoadWAV(filePath.c_str());

	if (ptr == nullptr) {
		throw std::runtime_error("Failed to load " + filePath);
	}

	mp[filePath] = ptr;

	std::cout << "Loaded " + filePath << '\n';

	return ptr;
}

void Audio::load_batch(std::vector<std::string> filePaths) {
	for (auto& filePath : filePaths) {
		load(filePath);
	}
}

Mix_Chunk* Audio::death() {
	return load("assets/player/death" + std::to_string(Utils::getRandom(1, 6)) + ".wav");
}

Mix_Chunk* Audio::kill() {
	return load("assets/player/kill_doof_01.wav");
}

Mix_Chunk* Audio::headshot_no_armor() {
	return load("assets/player/headshot_noarmor_0" + std::to_string(Utils::getRandom(1, 5)) + ".wav");
}

Mix_Chunk* Audio::headshot_armor() {
	return load("assets/player/headshot_armor_flesh.wav");
}

Mix_Chunk* Audio::headshot_armor_dink() {
	return load("assets/player/headshot_armor_e1.wav");
}

Mix_Chunk* Audio::bodyshot_no_armor() {
	return load("assets/player/player_damagebody_0" + std::to_string(Utils::getRandom(1, 5)) + ".wav");
}

Mix_Chunk* Audio::bodyshot_armor() {
	return load("assets/player/kevlar_0" + std::to_string(Utils::getRandom(1, 5)) + ".wav");
}


