#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "util/Helper.h"
#include "ecs/Registry.h"

MapLoader::MapLoader(const std::string& path)
  : m_MapsPath(path)
{
  if (!std::filesystem::is_directory(path))
	std::cout << "MapLoader: FAILED TO FIND MAPS DIRECTORY: " << path << '\n';
  else
	std::cout << "MapLoader: MAPS DIRECTORY FOUND\n";
}

std::optional<Map> MapLoader::LoadFile(const std::string& filename)
{
  if (!std::filesystem::exists(m_MapsPath / filename)) {
	std::cout << "MapLoader: MAP FILE '" << filename << "' DOES NOT EXIST\n";
	return {};
  }

  std::string line;
  std::ifstream file(m_MapsPath / filename);
  Map map;
  
  if (file.is_open()) {
	std::getline(file, line);
	map.playerInitialPos = adv::ToVector2(line);
	
	std::getline(file, line);
	map.grid = adv::ToVector2(line);

	Vector2 temp;
	for (size_t i = 0; i < map.grid.y && std::getline(file, line); i++) {
	  auto it = line.begin();
	  while (it != line.end()) {
		temp = adv::ToVector2(it, line.end());
		map.tiles[i].emplace_back(std::make_pair<size_t, size_t>(temp.x, temp.y));
		
		if (it == line.end()) break;
		it++;
	  }
	  map.tiles.emplace_back();
	}
	file.close();
  }

  return m_FileToMap[filename] = map;
}

Map& MapLoader::GetMap(const std::string& filename)
{
  assert(m_FileToMap.find(filename) != m_FileToMap.end() && "Map not loaded");
  return m_FileToMap[filename];
}
