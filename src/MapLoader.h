#pragma once
#include <filesystem>
#include <map>
#include <utility>
#include <vector>
#include <optional>
#include <raylib.h>

struct Tile
{
  Vector2 pos;
  size_t texture, spriteIndex;
};

struct Map
{
  Vector2 playerInitialPos, flagPos;
  Vector2 grid;
  size_t width;
  std::vector<Tile> tiles {};
};

class MapLoader
{
public:
  MapLoader(const std::string& path);

  std::optional<Map> LoadFile(const std::string&);
  Map& GetMap(const std::string& filename);
	
private:
  const std::filesystem::path m_MapsPath;
  std::map<const std::string, Map> m_FileToMap {};
};

