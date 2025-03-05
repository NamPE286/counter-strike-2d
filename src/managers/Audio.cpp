#include "Audio.hpp"

#include <iostream>

void Audio::init() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

Mix_Chunk* Audio::load(std::string filePath) {
	if (mp.contains(filePath)) {
		std::cout << "Already loaded " + filePath << '\n';
		return mp[filePath];
	}

	std::cout << "Loading " + filePath << '\n';

	auto* ptr = Mix_LoadWAV(filePath.c_str());
	mp[filePath] = ptr;

	std::cout << "Loaded " + filePath << '\n';

	return ptr;
}

void Audio::load_batch(std::vector<std::string> filePaths) {
	for (auto& filePath : filePaths) {
		load(filePath);
	}
}

void Audio::destroy() {
	for (auto& i : mp) {
		Mix_FreeChunk(i.second);
	}

	Mix_Quit();
}
