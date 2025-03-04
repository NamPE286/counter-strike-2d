#include "Bullet.hpp"
#include "../managers/Time.hpp"

Bullet::Bullet(SDL_Renderer* renderer, float angle, int x, int y):
	MonoBehaviour(renderer), angle(angle), x(x), y(y)
{}

void Bullet::update() {
	alpha -= int(255 * Time::deltaTime / 2000);
}

void Bullet::render() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);

	int length = 100;
	int endX = x + static_cast<int>(length * cos(angle));
	int endY = y + static_cast<int>(length * sin(angle));

	SDL_RenderDrawLine(renderer, x, y, endX, endY);
}

bool Bullet::finished() const {
	return alpha <= 0;
}