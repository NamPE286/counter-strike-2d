#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"

class Map : public MonoBehaviour {
	tmx_map *map = nullptr;
	SDL_Texture *texture = nullptr;
	std::vector<SDL_Point> cornerPoints;

	void set_color(int color);
	void render_all_layers(tmx_layer *layers);
	void render_image_layer(tmx_image *image);
	void render_layer(tmx_layer *layer);
	void render_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh, unsigned int dx, unsigned int dy, float opacity, unsigned int flags);
	void calc_corner_points();
	bool RayIntersectsRect(float originX, float originY, float dirX, float dirY, const SDL_Rect &rect, float &tEnter, float &tExit);
	tmx_tile* get_tile(int x, int y);
	tmx_layer *get_layer(std::string name);

public:
	int w = 0, h = 0;

	Map(SDL_Renderer* renderer, std::string filePath);
	~Map();

	void render();
	void render_visible_area(Player* p, std::vector<Player*> &players, bool renderLine = false);
	void collision_handler(Player *p);
	int distance(float originX, float originY, float angle, int length = INT_MAX, int step = 0);
	tmx_object *get_spawn(int side);
	tmx_object *get_bombsite(int index);
	tmx_object *get_callout(float x, float y);
};