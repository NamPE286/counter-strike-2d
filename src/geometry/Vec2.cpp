#include "Vec2.hpp"

Vec2::Vec2() {
	x = y = 0;
}

Vec2::Vec2(float _x, float _y) {
	x = _x;
	y = _y;
}

bool Vec2::operator==(Vec2 const& rhs) {
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(Vec2 const& rhs) {
	return !(x == rhs.x && y == rhs.y);
}

Vec2 Vec2::operator+(Vec2 const& rhs) {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2& Vec2::operator+=(Vec2 const& rhs) {
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(Vec2 const& rhs) {
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}


Vec2 Vec2::operator*(float rhs) {
	return Vec2(x * rhs, y * rhs);
}