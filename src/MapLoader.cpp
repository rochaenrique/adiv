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
	map.grid = adv::ToVector2(line); // grid to split the screen
	
	std::getline(file, line);
	map.playerInitialPos = adv::ToVector2(line); // player position (from right to left) in grid coords

	Vector2 temp;
	map.flagPos = { 0, 0 };
	for (size_t i = 0; std::getline(file, line); i++) {
	  auto it = line.begin();
	  while (it != line.end()) {
		temp = adv::ToVector2(it, line.end());
		
		if (temp.y == 0 && i > map.flagPos.y) {
		  map.flagPos.y = i+1;
		  std::cout << "Found flag pos at: " << map.flagPos << '\n';
		}

		map.tiles.emplace_back(Tile{ { temp.y, (float)i }, 0, (size_t)temp.x }); // temporary
		
		if (it == line.end()) break;
		it++;
	  }
	}
	file.close();
  }

  std::cout << "Read Map file '" << filename << "':"
	"\n\tgrid: " << map.grid <<
	"\n\tplayerPos: " << map.playerInitialPos <<
	"\n\tflagPos: " << map.flagPos << 
	"\n\ttiles: " << map.tiles.size() << '\n';

  return m_FileToMap[filename] = map;
}

Map& MapLoader::GetMap(const std::string& filename)
{
  assert(m_FileToMap.find(filename) != m_FileToMap.end() && "Map not loaded");
  return m_FileToMap[filename];
}
