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

		int a = (int)std::count(v.begin(), v.begin() + 4, true);
		int b = (int)std::count(v.begin() + 4, v.end(), true);

		if (a == 2) {
			cornerPoints.push_back({ x, y });
		} else if (a == 4 && b == 3) {
			cornerPoints.push_back({ x, y });
		}
	}

	cornerPoints.push_back({ 0, 0 });
	cornerPoints.push_back({ int(map->width * map->tile_width), 0 });
	cornerPoints.push_back({ 0, int(map->height * map->tile_height) });
	cornerPoints.push_back({ int(map->width * map->tile_width), int(map->height * map->tile_height) });
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
	if (texture == nullptr) {
		auto *tmp = SDL_GetRenderTarget(renderer);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetRenderTarget(renderer, texture);

		SDL_SetRenderDrawColor(renderer, 137, 137, 137, 255);
		SDL_RenderClear(renderer);
		set_color(map->backgroundcolor);
		render_all_layers(map->ly_head);

		SDL_SetRenderTarget(renderer, tmp);
	}

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, 0, 0);
}

void Map::render_visible_area(Player *p) {
	std::vector<SDL_FPoint> points;
	std::vector<float> offsets = { -0.05f, 0.0f, 0.05f };

	for (SDL_Point point : cornerPoints) {
		for (float offset : offsets) {
			int dx = point.x - static_cast<int>(p->position.x);
			int dy = point.y - static_cast<int>(p->position.y);
			float angle = static_cast<float>(atan2(dy, dx)) + offset;
			int maxLength;

			float rayDirX = cos(angle);
			float rayDirY = sin(angle);
			float tMin = 0.0f, tMax = std::numeric_limits<float>::max();

			if (rayDirX != 0.0f) {
				float tx1 = (0.0f - p->position.x) / rayDirX;
				float tx2 = (static_cast<float>(w) - p->position.x) / rayDirX;
				float tMinX = std::min(tx1, tx2);
				float tMaxX = std::max(tx1, tx2);
				tMin = std::max(tMin, tMinX);
				tMax = std::min(tMax, tMaxX);
			} else if (p->position.x <= 0.0f || p->position.x >= static_cast<float>(w)) {
				continue;
			}

			if (rayDirY != 0.0f) {
				float ty1 = (0.0f - p->position.y) / rayDirY;
				float ty2 = (static_cast<float>(h) - p->position.y) / rayDirY;
				float tMinY = std::min(ty1, ty2);
				float tMaxY = std::max(ty1, ty2);
				tMin = std::max(tMin, tMinY);
				tMax = std::min(tMax, tMaxY);
			} else if (p->position.y <= 0.0f || p->position.y >= static_cast<float>(h)) {
				continue;
			}

			if (tMax < tMin) {
				continue;
			}

			maxLength = static_cast<int>(tMax);

			int length = distance(p->position.x, p->position.y, angle, maxLength);

			if (offset == 0.0f) {
				length = std::min(length, Utils::getDistance({ static_cast<int>(p->position.x), static_cast<int>(p->position.y) }, point));
			}

			SDL_FPoint end = {
				p->position.x + static_cast<float>(length) * cos(angle),
				p->position.y + static_cast<float>(length) * sin(angle)
			};

			points.push_back(end);
			SDL_RenderDrawLine(renderer, static_cast<int>(p->position.x), static_cast<int>(p->position.y), static_cast<int>(end.x), static_cast<int>(end.y));
		}
	}

	std::sort(points.begin(), points.end(), [&](const SDL_FPoint &a, const SDL_FPoint &b) {
		float angleA = atan2(a.y - p->position.y, a.x - p->position.x);
		float angleB = atan2(b.y - p->position.y, b.x - p->position.x);
		return angleA < angleB;
	});
	points.push_back(points[0]);

	std::vector<SDL_Vertex> vertices;
	SDL_Color bgColor = { 0, 255, 255, 255 };

	for (size_t i = 1; i < points.size(); i++) {
		SDL_FPoint a = { p->position.x, p->position.y };
		SDL_FPoint b = points[i - 1];
		SDL_FPoint c = points[i];

		vertices.emplace_back(a, bgColor, SDL_FPoint{ a.x / (float)w, a.y / (float)h });
		vertices.emplace_back(b, bgColor, SDL_FPoint{ b.x / (float)w, b.y / (float)h });
		vertices.emplace_back(c, bgColor, SDL_FPoint{ c.x / (float)w, c.y / (float)h });
	}

	SDL_RenderGeometry(renderer, texture, vertices.data(), static_cast<int>(vertices.size()), nullptr, 0);
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

int Map::distance(float originX, float originY, float angle, int length, int step) {
	float dx = cos(angle);
	float dy = sin(angle);
	float minDistance = static_cast<float>(length);

	for (int tileY = 0; tileY < static_cast<int>(map->height); ++tileY) {
		for (int tileX = 0; tileX < static_cast<int>(map->width); ++tileX) {
			tmx_tile *tile = map->tiles[(map->ly_head->content.gids[(tileY * map->width) + tileX]) & TMX_FLIP_BITS_REMOVAL];

			if (tile && tile->collision) {
				SDL_Rect tileRect = {
					tileX * static_cast<int>(map->tile_width),
					tileY * static_cast<int>(map->tile_height),
					static_cast<int>(map->tile_width),
					static_cast<int>(map->tile_height)
				};

				float tEnter, tExit;

				if (RayIntersectsRect(originX, originY, dx, dy, tileRect, tEnter, tExit)) {
					if (tEnter >= 0.0f && tEnter < minDistance) {
						minDistance = tEnter;
					}
				}
			}
		}
	}

	return static_cast<int>(minDistance);
}

bool Map::RayIntersectsRect(float originX, float originY, float dirX, float dirY, const SDL_Rect &rect, float &tEnter, float &tExit) {
	float tMin = (rect.x - originX) / dirX;
	float tMax = ((rect.x + rect.w) - originX) / dirX;

	if (tMin > tMax) std::swap(tMin, tMax);

	float tyMin = (rect.y - originY) / dirY;
	float tyMax = ((rect.y + rect.h) - originY) / dirY;

	if (tyMin > tyMax) std::swap(tyMin, tyMax);

	if ((tMin > tyMax) || (tyMin > tMax))
		return false;

	if (tyMin > tMin)
		tMin = tyMin;

	if (tyMax < tMax)
		tMax = tyMax;

	tEnter = tMin;
	tExit = tMax;

	return tEnter <= tExit && tExit >= 0.0f;
}