#pragma once
#include <filesystem>
#include <map>
#include <utility>
#include <vector>
#include <optional>
#include <raylib.h>

struct Map
{
  Vector2 playerInitialPos;
  Vector2 grid;
  std::vector<std::vector<std::pair<size_t, size_t>>> tiles {{}};
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

