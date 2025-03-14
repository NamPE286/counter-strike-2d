#include "Map.hpp"

#include <stdexcept>
#include <iostream>

Map::Map(SDL_Renderer *renderer, std::string filePath):
	MonoBehaviour(renderer)
{
	//if (!map.load(filePath)) {
	//	throw std::runtime_error("Failed to load tilemap " + filePath);
	//}

	//const auto &tileSets = map.getTilesets();

 //   for (const auto &ts : tileSets) {
	//	std::cout << ts.getImagePath() << '\n';
 //   }
}

void Map::render() {
}
