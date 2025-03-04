#include "Bullet.hpp"

#include <iostream>

#include "../managers/Time.hpp"

Bullet::Bullet(SDL_Renderer* renderer, float angle, int x, int y, int length):
	MonoBehaviour(renderer), angle(angle), x(x), y(y), length(length)
{}

void Bullet::update() {
	alpha -= int(2 * Time::deltaTime);
}

void Bullet::render() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);

	int endX = x + static_cast<int>(length * cos(angle));
	int endY = y + static_cast<int>(length * sin(angle));

	SDL_RenderDrawLine(renderer, x, y, endX, endY);
}

bool Bullet::finished() const {
	return alpha <= 0;
}