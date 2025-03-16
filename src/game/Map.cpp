#include "Map.hpp"

#include <stdexcept>
#include <iostream>
#include <tmx.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>

#include "../utilities/Utils.hpp"

SDL_Renderer *_renderer = nullptr;

static void *SDL_tex_loader(const char *path) {
	auto *tex = IMG_LoadTexture(_renderer, path);

	if (tex == nullptr) {
		throw std::runtime_error("Failed to load image " + std::string(path));
	}

	return tex;
}

void Map::set_color(int color) {
	tmx_col_bytes col = tmx_col_to_bytes(color);
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
}

void Map::render_all_layers(tmx_layer *layers) {
	while (layers) {
		if (layers->visible) {
			if (layers->type == L_GROUP) {
				render_all_layers(layers->content.group_head);
			} else if (layers->type == L_OBJGR) {
				render_objects(layers->content.objgr);
			} else if (layers->type == L_IMAGE) {
				render_image_layer(layers->content.image);
			} else if (layers->type == L_LAYER) {
				render_layer(layers);
			}
		}
		layers = layers->next;
	}
}

void Map::render_image_layer(tmx_image *image) {
	SDL_Rect dim;
	dim.x = dim.y = 0;

	SDL_Texture *texture = (SDL_Texture *)image->resource_image;
	SDL_QueryTexture(texture, NULL, NULL, &(dim.w), &(dim.h));
	SDL_RenderCopy(renderer, texture, NULL, &dim);
}

void Map::render_layer(tmx_layer *layer) {
	unsigned long i, j;
	unsigned int gid, x, y, w, h, flags;
	float op;
	tmx_tileset *ts;
	tmx_image *im;
	void *image;
	op = (float)layer->opacity;
	for (i = 0; i < map->height; i++) {
		for (j = 0; j < map->width; j++) {
			gid = (layer->content.gids[(i * map->width) + j]) & TMX_FLIP_BITS_REMOVAL;

			if (map->tiles[gid] != NULL) {
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				x = map->tiles[gid]->ul_x;
				y = map->tiles[gid]->ul_y;
				w = ts->tile_width;
				h = ts->tile_height;
				if (im) {
					image = im->resource_image;
				} else {
					image = ts->image->resource_image;
				}
				flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
				render_tile(image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
			}
		}
	}
}

void Map::render_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh, unsigned int dx, unsigned int dy, float opacity, unsigned int flags) {
	SDL_Rect src_rect, dest_rect;
	src_rect.x = sx;
	src_rect.y = sy;
	src_rect.w = dest_rect.w = sw;
	src_rect.h = dest_rect.h = sh;
	dest_rect.x = dx;
	dest_rect.y = dy;
	SDL_RenderCopy(renderer, (SDL_Texture *)image, &src_rect, &dest_rect);
}

void Map::render_objects(tmx_object_group *objgr) {
	SDL_Rect rect;
	set_color(objgr->color);
	tmx_object *head = objgr->head;
	while (head) {
		if (head->visible) {
			if (head->obj_type == OT_SQUARE) {
				rect.x = (int)head->x;  rect.y = (int)head->y;
				rect.w = (int)head->width;  rect.h = (int)head->height;
				SDL_RenderDrawRect(renderer, &rect);
			} else if (head->obj_type == OT_POLYGON) {
				render_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			} else if (head->obj_type == OT_POLYLINE) {
				render_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			}
		}
		head = head->next;
	}
}

void Map::render_polyline(double **points, double x, double y, int pointsc) {
	for (int i = 1; i < pointsc; i++) {
		SDL_RenderDrawLine(renderer, int(x + points[i - 1][0]), int(y + points[i - 1][1]), int(x + points[i][0]), int(y + points[i][1]));
	}
}

void Map::render_polygon(double **points, double x, double y, int pointsc) {
	render_polyline(points, x, y, pointsc);
	if (pointsc > 2) {
		SDL_RenderDrawLine(renderer, int(x + points[0][0]), int(y + points[0][1]), int(x + points[pointsc - 1][0]), int(y + points[pointsc - 1][1]));
	}
}

void Map::calc_corner_points() {
	cornerPoints.clear();

	std::set<std::pair<int, int>> s;

	for (int y = 0; y < (int)map->height; ++y) {
		for (int x = 0; x < (int)map->width; ++x) {
			int gid = (map->ly_head->content.gids[(y * map->width) + x]) & TMX_FLIP_BITS_REMOVAL;
			tmx_tile *tile = map->tiles[gid];

			if (tile && tile->collision) {
				int tileX = x * map->tile_width;
				int tileY = y * map->tile_height;

				s.emplace(tileX, tileY);
				s.emplace(tileX + map->tile_width, tileY);
				s.emplace(tileX, tileY + map->tile_height);
				s.emplace(tileX + map->tile_width, tileY + map->tile_height);
			}
		}
	}

	for (auto &[x, y] : s) {
		std::vector<bool> v = {
			s.contains({x, y - map->tile_height}),
			s.contains({x - map->tile_width, y}),
			s.contains({x + map->tile_width, y}),
			s.contains({x, y + map->tile_height}),
			s.contains({x + map->tile_width, y - map->tile_height}),
			s.contains({x + map->tile_width, y + map->tile_height}),
			s.contains({x - map->tile_width, y - map->tile_height}),
			s.contains({x - map->tile_width, y + map->tile_height}),
		};

		if (std::count(v.begin(), v.begin() + 4, true) == 2 || std::count(v.begin() + 4, v.end(), true) == 3) {
			cornerPoints.push_back({ x, y });
		}
	}
}

tmx_tile *Map::get_tile(int x, int y) {
	int tileX = x / map->tile_width;
	int tileY = y / map->tile_height;

	if (tileX < 0 || tileX >= (int)map->width || tileY < 0 || tileY >= (int)map->height) {
		return nullptr;
	}

	int gid = (map->ly_head->content.gids[(tileY * map->width) + tileX]) & TMX_FLIP_BITS_REMOVAL;

	return map->tiles[gid];
}

Map::Map(SDL_Renderer *renderer, std::string filePath):
	MonoBehaviour(renderer)
{
	_renderer = renderer;
	tmx_img_load_func = SDL_tex_loader;
	tmx_img_free_func = (void (*)(void *))SDL_DestroyTexture;

	map = tmx_load(filePath.c_str());

	if (map == nullptr) {
		tmx_perror("Cannot load map");
		throw std::runtime_error("Failed to load tilemap " + filePath);
	}

	w = map->width * map->tile_width, h = map->height * map->tile_height;
	calc_corner_points();
}

Map::~Map() {
	tmx_map_free(map);
}

void Map::render() {
	set_color(map->backgroundcolor);
	render_all_layers(map->ly_head);
}

void Map::render_shadow(Player *p) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

	std::vector<SDL_Point> points;
	std::vector<float> offsets = { -0.01f, 0, 0.01f };

	for (SDL_Point point : cornerPoints) {
		for (float offset : offsets) {
			int dx = point.x - static_cast<int>(p->position.x);
			int dy = point.y - static_cast<int>(p->position.y);
			float angle = (float)atan2(dy, dx);
			int length = distance((int)p->position.x, (int)p->position.y, angle + offset, 1000);
			SDL_Point end = {
				static_cast<int>(p->position.x + length * cos(angle + offset)),
				static_cast<int>(p->position.y + length * sin(angle + offset))
			};

			points.push_back({ end.x, end.y });
			SDL_RenderDrawLine(renderer, (int)p->position.x, (int)p->position.y, end.x, end.y);
		}
	}

	std::sort(points.begin(), points.end(), [&](const SDL_Point &a, const SDL_Point &b) -> bool {
		float angle_a = std::atan2(static_cast<float>(a.y) - p->position.y, static_cast<float>(a.x) - p->position.x);
		float angle_b = std::atan2(static_cast<float>(b.y) - p->position.y, static_cast<float>(b.x) - p->position.x);

		return angle_a < angle_b;
	});

	for (size_t i = 1; i < points.size(); i++) {
		SDL_Point a = { (int)p->position.x, (int)p->position.y }, b = points[i - 1], c = points[i];
		SDL_Vertex vertices[3] = {
				{ SDL_FPoint{ (float)a.x, (float)a.y }, SDL_Color{ 0, 255, 255, 255 }, SDL_FPoint{ 0, 0 } },
				{ SDL_FPoint{ (float)b.x, (float)b.y }, SDL_Color{ 0, 255, 255, 255 }, SDL_FPoint{ 0, 0 } },
				{ SDL_FPoint{ (float)c.x, (float)c.y }, SDL_Color{ 0, 255, 255, 255 }, SDL_FPoint{ 0, 0 } }
		};
		SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
	}
}

void Map::collision_handler(Player *p) {
	tmx_layer *layer = map->ly_head;
	std::vector<std::pair<int, int>> corners = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
	SDL_Rect playerRect = { static_cast<int>(p->position.x) - p->size / 2,
							static_cast<int>(p->position.y) - p->size / 2,
							p->size,
							p->size };

	for (auto &[a, b] : corners) {
		int x = (playerRect.x + a * playerRect.w) / map->tile_width;
		int y = (playerRect.y + b * playerRect.h) / map->tile_height;
		int gid = (layer->content.gids[(y * map->width) + x]) & TMX_FLIP_BITS_REMOVAL;
		tmx_tile *tile = map->tiles[gid];
		int cnt = 0;

		if (tile->collision != nullptr) {
			cnt++;
			SDL_Rect collisionRect = { x * (int)map->tile_width + (int)tile->collision->x,
										y * (int)map->tile_height + (int)tile->collision->y,
										(int)tile->collision->width,
										(int)tile->collision->height };

			if (SDL_HasIntersection(&playerRect, &collisionRect)) {
				p->position = p->prevPosition;

				if (p->velocity.x > 0) {
					playerRect.x -= 20;
				} else if (p->velocity.x < 0) {
					playerRect.x += 20;
				}

				if (p->velocity.x != 0.0f && !SDL_HasIntersection(&playerRect, &collisionRect)) {
					p->stop_movement_x();
					return;
				}

				if (p->velocity.y > 0) {
					playerRect.y -= 20;
				} else if (p->velocity.y < 0) {
					playerRect.y += 20;
				}

				if (p->velocity.y != 0.0f && !SDL_HasIntersection(&playerRect, &collisionRect)) {
					p->stop_movement_y();
				}

				return;
			}
		}
	}
}

int Map::distance(int originX, int originY, float angle, int length) {
	for (int i = 0; i <= length; i += 1) {
		int x = originX + int((float)i * cos(angle));
		int y = originY + int((float)i * sin(angle));
		auto *tile = get_tile(x, y);

		if (tile && tile->collision) {
			int tileX = x / map->tile_width;
			int tileY = y / map->tile_height;
			int offset = 1;

			SDL_Rect collisionRect = { tileX * (int)map->tile_width - offset,
										tileY * (int)map->tile_height - offset,
										(int)map->tile_width + 2 * offset,
										(int)map->tile_height + 2 * offset };

			return Utils::getDistance({ originX, originY }, Utils::getIntersection(originX, originY, angle, collisionRect));
		}
	}

	return length;
}
