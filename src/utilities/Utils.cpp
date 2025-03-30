#include "Utils.hpp"

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

float Utils::cross_product(Vec2 a, Vec2 b, Vec2 c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

std::vector<SDL_Point> Utils::convex_hull(std::vector<SDL_Point> points) {
	if (points.size() < 3)
		return points;

	std::sort(points.begin(), points.end(), [](const SDL_Point &a, const SDL_Point &b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
		});

	auto cross = [](const SDL_Point &O, const SDL_Point &A, const SDL_Point &B) -> int {
		return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
		};

	std::vector<SDL_Point> hull;

	for (const auto &p : points) {
		while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0)
			hull.pop_back();
		hull.push_back(p);
	}

	size_t t = hull.size() + 1;

	for (auto it = points.rbegin(); it != points.rend(); ++it) {
		const auto &p = *it;
		while (hull.size() >= t && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0)
			hull.pop_back();
		hull.push_back(p);
	}

	hull.pop_back();

	return hull;
}

