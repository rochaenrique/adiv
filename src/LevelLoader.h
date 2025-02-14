#pragma once
#include "Level.h"
#include "util/Helper.h"
#include "ecs/components/Player.h"
#include <filesystem>
#include <map>
#include <utility>
#include <vector>
#include <optional>
#include <cassert>
#include <raylib.h>

#define FRAMES_TO_CHANGE 5

class LevelLoader
{
public:
  LevelLoader(const std::string& path);

  void LoadFile(const std::string&);

  void InitLevels();
  bool NextLevel();
  const std::unique_ptr<Level>& GetCurrent() const;
  const std::shared_ptr<Camera2D> GetCamera() const;
  
private:
  const std::filesystem::path m_LevelsPath;
  std::vector<std::unique_ptr<Level>> m_Levels {};
  
  using LevelsIt = std::vector<std::unique_ptr<Level>>::iterator;
  LevelsIt m_CurrentLevel;
  size_t m_FramesTillChange = FRAMES_TO_CHANGE;
  
  static void LoadTexturePack(std::ifstream&, Level&, TextureType);
  static void LoadTiles(std::ifstream&, Map&);
  static void LoadPlayerAnimator(std::ifstream&, Level&);
  static adv::Animation LoadPlayerAnimation(std::ifstream&);
};

