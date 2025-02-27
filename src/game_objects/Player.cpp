#include "Player.hpp"

Player::Player(SDL_Renderer* renderer, Vec2 pos): 
	MonoBehaviour(renderer), pos(pos)
{}

void Player::render() {
	SDL_Rect rect = { (int)pos.x, (int)pos.y, 10, 10 };

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}
