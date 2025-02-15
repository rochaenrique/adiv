#include "LevelLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "ecs/Registry.h"
#include "ecs/components/Animator.h"

#include "EventManager.h"

#define SEPARATOR "###"

LevelLoader::LevelLoader(const std::string& path)
  : m_LevelsPath(path)
{
  if (!std::filesystem::is_directory(path))
	std::cout << "LevelLoader: FAILED TO FIND MAPS DIRECTORY: " << path << '\n';
  else
	std::cout << "LevelLoader: MAPS DIRECTORY FOUND\n";
}

void LevelLoader::LoadFile(const std::string& filename)
{
  if (!std::filesystem::exists(m_LevelsPath / filename)) {
	std::cout << "LevelLoader: MAP FILE '" << filename << "' DOES NOT EXIST\n";
	return;
  }

  Level level(filename);
  Map& map = level.GetMap();

  std::ifstream file(m_LevelsPath / filename);
  std::string line;
  if (!file.is_open()) return;
  std::getline(file, line);
  map.grid = adv::ToVector2(line); // grid to split the screen

  std::getline(file, line);
  map.flagPos = adv::ToVector2(line); // flag offset

  LoadTexturePack(file, level, TextureType::PLAYER);
  LoadTexturePack(file, level, TextureType::FLAG);
  LoadTexturePack(file, level, TextureType::TILE);

  LoadTiles(file, map);

  // animation
  LoadPlayerAnimator(file, level);
  
  std::cout << "Read Map file '" << filename << "':"
	"\n\tgrid: " << map.grid <<
	"\n\tplayerPos: " << map.playerInitialPos <<
	"\n\tflagPos: " << map.flagPos << 
	"\n\ttiles: " << map.tiles.size() <<
	"\n\twidth: " << map.width << '\n';
  file.close();
  m_Levels.emplace_back(std::make_unique<Level>(level));
}

void LevelLoader::InitLevels()
{
  assert(!m_Levels.empty() && "No levels");
  m_CurrentLevel = m_Levels.begin();
  (*m_CurrentLevel)->Load();
}

bool LevelLoader::NextLevel()
{
  static bool reached = false;
  bool res = false;
  
  if (!reached) {
	std::cout << "Just unloaded level\n";
	(*m_CurrentLevel)->Unload();
	m_CurrentLevel++;
	reached = true;
  }
  
  if ((res = (*m_CurrentLevel)->AttemptLoad()))
	reached = false;
	
  EventManager::Get().Flush(EventType::CheckPoint);
  return res;
}

const std::unique_ptr<Level>& LevelLoader::GetCurrent() const
{
  assert(m_CurrentLevel != m_Levels.end() && "Current level is invalid");
  return *m_CurrentLevel;
}

const std::shared_ptr<Camera2D> LevelLoader::GetCamera() const
{
  assert(m_CurrentLevel != m_Levels.end() && "Current level is invalid");
  return (*m_CurrentLevel)->GetCamera().camera;
}

void LevelLoader::LoadTexturePack(std::ifstream& file, Level& level, TextureType type)
{
  std::string line, textureFile;
  
  std::getline(file, textureFile);
  std::getline(file, line);
  
  auto it = line.begin();
  Vector2 grid = adv::ToVector2(it, line.end());
  float scale = std::stof(std::string(it, line.end()));
  // std::cout << "Texture:\n\tfile: " << textureFile <<
  // 	"\n\tgrid: " << grid << "\n\tscale: " << scale << '\n';

  level.AddTexturePack(textureFile, grid, scale, type);
}

void LevelLoader::LoadTiles(std::ifstream& file, Map& map)
{
  Vector2 flagPos = { 0, -1 };
  map.playerInitialPos = { 0, -1 };

  map.width = 0;
  
  std::string line;
  Vector2 temp;
  for (size_t i = 0; std::getline(file, line) && line != SEPARATOR; i++) {
	auto it = line.begin();
	while (it != line.end()) {
	  temp = adv::ToVector2(it, line.end());

	  if (i > map.flagPos.y) map.flagPos.y = i+1;
	  if (temp.y > map.width) map.width = temp.y;
	  if (temp.y >= map.playerInitialPos.x) 
		map.playerInitialPos = { temp.y, static_cast<float>(i+2) };

	  map.tiles.emplace_back(Tile{ { temp.y, static_cast<float>(i) }, 0, static_cast<size_t>(temp.x) }); // temporary
		
	  if (it == line.end()) break;
	  it++;
	}
  }
  map.width++;
  map.flagPos -= flagPos;
}

void LevelLoader::LoadPlayerAnimator(std::ifstream& file, Level& level)
{
  adv::Animator& animator = level.GetPlayerAnimator();
  animator.Insert(adv::PlayerState::WALK_LEFT,	LoadPlayerAnimation(file));
  animator.Insert(adv::PlayerState::WALK_RIGHT, LoadPlayerAnimation(file));
  animator.Insert(adv::PlayerState::JUMP,		LoadPlayerAnimation(file));
  animator.Insert(adv::PlayerState::IDLE,		LoadPlayerAnimation(file));
  animator.ChangeTo(adv::PlayerState::IDLE);
}

adv::Animation LevelLoader::LoadPlayerAnimation(std::ifstream& file)
{
  std::string line;
  std::getline(file, line);
  
  auto it = line.begin();
  Vector2 fromTo = adv::ToVector2(it, line.end());
  float duration = std::stof(std::string(it, line.end()));
  
  return adv::Animation({{ fromTo.x, fromTo.y, duration }});
}
