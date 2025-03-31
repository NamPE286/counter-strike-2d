#include "Audio.hpp"

#include <iostream>
#include <stdexcept>

#include "../utilities/Utils.hpp"

void Audio::init() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error("Failed to initialize SDL_mixer. Error: " + std::string(Mix_GetError()));
	}

	if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
		throw std::runtime_error("Failed to initialize MP#. Error: " + std::string(Mix_GetError()));
	}
}

void Audio::destroy() {
	for (auto &i : wavMp) {
		Mix_FreeChunk(i.second);
	}

	Mix_Quit();
}

Mix_Chunk *Audio::loadWAV(std::string filePath) {
	if (wavMp.contains(filePath)) {
		return wavMp[filePath];
	}

	std::cout << "Loading " + filePath << '\n';

	auto *ptr = Mix_LoadWAV(filePath.c_str());

	if (ptr == nullptr) {
		throw std::runtime_error("Failed to load " + filePath);
	}

	wavMp[filePath] = ptr;

	std::cout << "Loaded " + filePath << '\n';

	return ptr;
}

Mix_Music *Audio::loadMusic(std::string filePath) {
	if (mp3Mp.contains(filePath)) {
		return mp3Mp[filePath];
	}

	std::cout << "Loading " + filePath << '\n';

	auto *ptr = Mix_LoadMUS(filePath.c_str());

	if (ptr == nullptr) {
		throw std::runtime_error("Failed to load " + filePath);
	}

	mp3Mp[filePath] = ptr;

	std::cout << "Loaded " + filePath << '\n';

	return ptr;
}

void Audio::load_batch(std::vector<std::string> filePaths) {
	for (auto &filePath : filePaths) {
		loadWAV(filePath);
	}
}

int Audio::playWAV(Mix_Chunk *chunk, Vec2 src, int channel, float multiplier) {
	channel = Mix_PlayChannel(channel, chunk, 0);

	if (channel == -1) {
		return -1;
	}

	Vec2 v = src + *dest * -1;
	v = v.normalized();

	float distance = std::sqrt((src.x - dest->x) * (src.x - dest->x) + (src.y - dest->y) * (src.y - dest->y));
	float volume = std::max(0.0f, 255.0f - distance / multiplier) / 255;

	float angle = std::atan2(v.y, v.x);
	float l = 255.0f * (1.0f - std::cos(angle)) / 2.0f;
	float r = 255.0f * (1.0f + std::cos(angle)) / 2.0f;

	if (distance <= 5.0f) {
		Mix_SetPanning(channel, 255, 255);
	} else {
		Mix_SetPanning(channel, int(l * volume), int(r * volume));
	}

	return channel;
}

Mix_Chunk *Audio::death() {
	return loadWAV("assets/player/death" + std::to_string(Utils::getRandomRange(1, 6)) + ".wav");
}

Mix_Chunk *Audio::kill() {
	return loadWAV("assets/player/kill_doof_01.wav");
}

Mix_Chunk *Audio::headshot_no_armor() {
	return loadWAV("assets/player/headshot_noarmor_0" + std::to_string(Utils::getRandomRange(1, 5)) + ".wav");
}

Mix_Chunk *Audio::headshot_armor() {
	return loadWAV("assets/player/headshot_armor_flesh.wav");
}

Mix_Chunk *Audio::headshot_armor_dink() {
	return loadWAV("assets/player/headshot_armor_e1.wav");
}

Mix_Chunk *Audio::bodyshot_no_armor() {
	return loadWAV("assets/player/player_damagebody_0" + std::to_string(Utils::getRandomRange(1, 5)) + ".wav");
}

Mix_Chunk *Audio::bodyshot_armor() {
	return loadWAV("assets/player/kevlar_0" + std::to_string(Utils::getRandomRange(1, 5)) + ".wav");
}


