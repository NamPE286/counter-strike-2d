#pragma once

#include <string>

namespace Utils {
	float getAngle(int x1, int y1, int x2, int y2);
	float getRandom(float variance);
	int getRandomRange(int start, int end);
	std::string getClockString(int m, int s);
}