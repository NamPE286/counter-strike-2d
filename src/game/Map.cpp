#include "Map.hpp"

#include <algorithm>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <set>
#include <stdexcept>
#include <tmx.h>
#include <utility>
#include <vector>

#include "../managers/Mouse.hpp"
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

void Map::calc_corner_points() {
	cornerPoints.clear();

	std::set<std::pair<int, int>> s;
	auto *layer = get_layer("Wall");

	for (int y = 0; y < (int)map->height; ++y) {
		for (int x = 0; x < (int)map->width; ++x) {
			int gid = (layer->content.gids[(y * map->width) + x]) & TMX_FLIP_BITS_REMOVAL;
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

tmx_layer *Map::get_layer(std::string name) {
	tmx_layer *layer = map->ly_head;

	while (layer) {
		if (std::string(layer->name) == name) {
			return layer;
		}

		layer = layer->next;
	}

	return nullptr;
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
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	calc_corner_points();
}

Map::~Map() {
	tmx_map_free(map);
}

void Map::render() {
	if (mapTexture == nullptr) {
		mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		auto *tmp = SDL_GetRenderTarget(renderer);

		SDL_SetRenderTarget(renderer, mapTexture);

		set_color(map->backgroundcolor);
		SDL_RenderClear(renderer);
		render_all_layers(map->ly_head);

		SDL_SetRenderTarget(renderer, tmp);
	}

	SDL_SetTextureColorMod(mapTexture, 220, 220, 220);
	SDL_RenderCopy(renderer, mapTexture, 0, 0);
	SDL_SetTextureColorMod(mapTexture, 255, 255, 255);
}

void Map::render_visible_area(Player *p, std::vector<Player *> &players, bool renderLine) {
	std::vector<SDL_Point> pointTmp = cornerPoints;
	std::vector<std::pair<float, float>> points;
	std::vector<float> offsets = { 0.0f, -0.05f, 0.05f };
	float mouseAngle = Utils::getAngle((int)p->position.x, (int)p->position.y, Mouse::x, Mouse::y);
	float fov = 2.35619;

	pointTmp.push_back({ int(p->position.x + 10000.0f * cos(mouseAngle - fov / 2)), int(p->position.y + 10000.0f * sin(mouseAngle - fov / 2)) });
	pointTmp.push_back({ int(p->position.x + 10000.0f * cos(mouseAngle + fov / 2)), int(p->position.y + 10000.0f * sin(mouseAngle + fov / 2)) });

	std::reverse(pointTmp.begin(), pointTmp.end());

	for (SDL_Point point : pointTmp) {
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

			std::pair<float, float> end = {
				p->position.x + static_cast<float>(length) * cos(angle),
				p->position.y + static_cast<float>(length) * sin(angle)
			};

			points.push_back(end);

			if (points.size() <= 6) {
				break;
			}
		}
	}

	std::pair<float, float> r0 = points[0];
	std::pair<float, float> r1 = points[1];

	std::sort(points.begin(), points.end(), [&](const std::pair<float, float> &a, const std::pair<float, float> &b) {
		bool res = (Utils::cross_product({ p->position.x, p->position.y }, { a.first, a.second }, { b.first, b.second }) > 0);
		return res;
	});

	auto it1 = std::find(points.begin(), points.end(), r0);
	auto it2 = std::find(points.begin(), points.end(), r1);

	if (it1 > it2) {
		points.erase(it1 + 1, points.end());
		points.erase(points.begin(), it2);
	} else {
		points.erase(it1 + 1, it2);

		it1 = std::find(points.begin(), points.end(), r0);
		it2 = std::find(points.begin(), points.end(), r1);

		std::reverse(points.begin(), it1 + 1);
		std::reverse(it2, points.end());
	}

	std::vector<SDL_Vertex> vertices;
	SDL_Color bgColor = { 255, 255, 255, 255 };

	for (size_t i = 1; i < points.size(); i++) {
		SDL_FPoint a = { p->position.x, p->position.y };
		SDL_FPoint b = { points[i - 1].first, points[i - 1].second };
		SDL_FPoint c = { points[i].first, points[i].second };

		vertices.emplace_back(a, bgColor, SDL_FPoint{ a.x / (float)w, a.y / (float)h });
		vertices.emplace_back(b, bgColor, SDL_FPoint{ b.x / (float)w, b.y / (float)h });
		vertices.emplace_back(c, bgColor, SDL_FPoint{ c.x / (float)w, c.y / (float)h });
	}

	auto *tmp = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr);

	for (auto *p : players) {
		p->render();
	}

	SDL_SetRenderTarget(renderer, tmp);
	SDL_RenderGeometry(renderer, texture, vertices.data(), static_cast<int>(vertices.size()), nullptr, 0);

	p->render();

	if (!renderLine) {
		return;
	}

	for (auto &i : points) {
		SDL_RenderDrawLine(renderer, static_cast<int>(p->position.x), static_cast<int>(p->position.y), static_cast<int>(i.first), static_cast<int>(i.second));
	}
}

void Map::collision_handler(Player *p) {
	tmx_layer *layer = get_layer("Wall");
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

		if (tile == nullptr || tile->collision == nullptr) {
			continue;
		}

		cnt++;
		SDL_Rect collisionRect = { x * (int)map->tile_width,
									y * (int)map->tile_height,
									(int)map->tile_width,
									(int)map->tile_height };

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

int Map::distance(float originX, float originY, float angle, int length, int step) {
	auto *layer = get_layer("Wall");
	float dx = cos(angle);
	float dy = sin(angle);

	for (int len = 1; len <= length; len++) {
		float x = originX + len * dx;
		float y = originY + len * dy;

		int tileX = static_cast<int>(x) / map->tile_width;
		int tileY = static_cast<int>(y) / map->tile_height;

		if (tileX < 0 || tileX >= static_cast<int>(map->width) || tileY < 0 || tileY >= static_cast<int>(map->height)) {
			return len;
		}

		int gid = (layer->content.gids[(tileY * map->width) + tileX]) & TMX_FLIP_BITS_REMOVAL;
		tmx_tile *tile = map->tiles[gid];

		if (tile && tile->collision) {
			if (step != 0.0f) {
				len += (float)step;
				step = 0.0f;
			} else {
				return len;
			}
		}
	}

	return length;
}

tmx_object *Map::get_spawn(int side) {
	auto *objgr = get_layer("Spawn")->content.objgr;
	auto *obj = objgr->head;

	while (obj) {
		if (side == PlayerSide::T && std::string(obj->name) == "T") {
			return obj;
		}

		if (side == PlayerSide::CT && std::string(obj->name) == "CT") {
			return obj;
		}

		obj = obj->next;
	}

	return nullptr;
}

tmx_object *Map::get_bombsite(int index) {
	auto *objgr = get_layer("Bombsite")->content.objgr;
	auto *obj = objgr->head;

	while (obj) {
		if (std::string(obj->name)[0] == ('A' + index)) {
			return obj;
		}

		obj = obj->next;
	}

	return nullptr;
}

tmx_object *Map::get_callout(float x, float y) {
	auto *objgr = get_layer("Callout")->content.objgr;
	auto *obj = objgr->head;

	while (obj) {
		SDL_FRect rect = {
			obj->x, obj->y,
			obj->width, obj->height
		};
		SDL_FPoint point = { x, y };

		if (SDL_PointInFRect(&point, &rect)) {
			return obj;
		}

		obj = obj->next;
	}

	return nullptr;
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