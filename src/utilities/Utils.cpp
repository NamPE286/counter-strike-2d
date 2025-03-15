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

std::string Utils::getClockString(int m, int s) {
	std::string a = std::to_string(m);
	std::string b = std::to_string(s);

	if (b.size() != 2) {
		b.insert(b.begin(), '0');
	}

	return a + ":" + b;
}

bool Utils::isPointInsideRect(const SDL_Point &point, const SDL_Rect &rect) {
	return (point.x >= rect.x) &&
		(point.x < rect.x + rect.w) &&
		(point.y >= rect.y) &&
		(point.y < rect.y + rect.h);
}

SDL_Point Utils::getIntersection(int x, int y, float angle, SDL_Rect rect) {
	SDL_Point intersection = { 0, 0 };
	float dx = std::cos(angle);
	float dy = std::sin(angle);
	float t_min = FLT_MAX;
	bool found = false;

	if (std::fabs(dx) > 1e-6f) {
		float t = (rect.x - x) / dx;
		if (t > 0) {
			float iy = y + dy * t;
			if (iy >= rect.y && iy <= rect.y + rect.h) {
				if (t < t_min) {
					t_min = t;
					intersection.x = rect.x;
					intersection.y = static_cast<int>(iy);
					found = true;
				}
			}
		}
	}

	if (std::fabs(dx) > 1e-6f) {
		float t = (rect.x + rect.w - x) / dx;
		if (t > 0) {
			float iy = y + dy * t;
			if (iy >= rect.y && iy <= rect.y + rect.h) {
				if (t < t_min) {
					t_min = t;
					intersection.x = rect.x + rect.w;
					intersection.y = static_cast<int>(iy);
					found = true;
				}
			}
		}
	}

	if (std::fabs(dy) > 1e-6f) {
		float t = (rect.y - y) / dy;
		if (t > 0) {
			float ix = x + dx * t;
			if (ix >= rect.x && ix <= rect.x + rect.w) {
				if (t < t_min) {
					t_min = t;
					intersection.x = static_cast<int>(ix);
					intersection.y = rect.y;
					found = true;
				}
			}
		}
	}

	if (std::fabs(dy) > 1e-6f) {
		float t = (rect.y + rect.h - y) / dy;
		if (t > 0) {
			float ix = x + dx * t;
			if (ix >= rect.x && ix <= rect.x + rect.w) {
				if (t < t_min) {
					t_min = t;
					intersection.x = static_cast<int>(ix);
					intersection.y = rect.y + rect.h;
					found = true;
				}
			}
		}
	}

	return intersection;
}

int Utils::getDistance(SDL_Point a, SDL_Point b) {
	int deltaX = b.x - a.x;
	int deltaY = b.y - a.y;

	return static_cast<int>(std::sqrt(deltaX * deltaX + deltaY * deltaY));
}
