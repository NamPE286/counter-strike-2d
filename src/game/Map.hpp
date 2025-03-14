#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

#include "../behaviours/MonoBehaviour.hpp"
#include "../gameObjects/Player.hpp"

class Map : public MonoBehaviour {
	tmx_map *map = nullptr;

	void set_color(int color);
	void render_all_layers(tmx_layer *layers);
	void render_image_layer(tmx_image *image);
	void render_layer(tmx_layer *layer);
	void render_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh, unsigned int dx, unsigned int dy, float opacity, unsigned int flags);
	void render_objects(tmx_object_group *objgr);
	void render_polyline(double **points, double x, double y, int pointsc);
	void render_polygon(double **points, double x, double y, int pointsc);

public:
	Map(SDL_Renderer* renderer, std::string filePath);
	~Map();

	void render();
	void collision_handler(Player *p);
};