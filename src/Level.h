#pragma once
#include <raylib.h>
#include <memory>
#include "MapLoader.h"
#include "ecs/ECS.h"
#include "ecs/components/Sprite.h"
#include "ecs/components/Camera.h"
#include <raymath.h>

class Level
{
public:
  Level(const Map&, const adv::Sprite&, const adv::Sprite&, const adv::Sprite&);

  void Load();
  void UnloadECS();

  const adv::Camera& GetCamera() const { return m_Camera; }
  
private:
  const Map& m_Map;
  const adv::Sprite m_PlayerSprite;
  const adv::Sprite m_TileSprite;
  const adv::Sprite m_FlagSprite;

  adv::Camera m_Camera;
  std::vector<Entity> m_Entities {};
};
