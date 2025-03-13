#include "Utils.hpp"

#include <numbers>
#include <cmath>
#include <random>

float Utils::getAngle(int x1, int y1, int x2, int y2) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	float angle = (float)std::atan2(deltaY, deltaX);

	return angle;
}

float Utils::getRandom(float variance) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(-variance, variance);

	return dis(gen);
}

int Utils::getRandomRange(int start, int end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis(start, end);

	return dis(gen);
}
