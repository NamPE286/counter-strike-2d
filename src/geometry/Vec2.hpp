#pragma once

class Vec2 {
public:
	float x, y;

	Vec2();
	Vec2(float _x, float _y);

	float magnitude() const;
	Vec2 normalized();
	float dot(const Vec2 &rhs) const;

	bool operator==(Vec2 const &rhs);
	bool operator!=(Vec2 const &rhs);
	Vec2 operator+(Vec2 const &rhs);
	Vec2 &operator+=(Vec2 const &rhs);
	Vec2 operator*(float rhs);
	Vec2 &operator*=(Vec2 const &rhs);
};