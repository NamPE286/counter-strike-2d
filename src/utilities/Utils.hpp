#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Utils {
	float getAngle(int x1, int y1, int x2, int y2);
	float getRandom(float variance);
	int getRandomRange(int start, int end);
	std::string getClockString(int m, int s);
	static bool isPointInsideRect(const SDL_Point &point, const SDL_Rect &rect);
	SDL_Point getIntersection(int x, int y, float angle, SDL_Rect rect);
	int getDistance(SDL_Point a, SDL_Point b);
}