#pragma once
#include <raylib.h>
#include <memory>
#include "MapLoader.h"
#include "ecs/ECS.h"
#include "ecs/components/Sprite.h"
#include <raymath.h>

class Level
{
public:
  Level(const Map&, const adv::Sprite&, const adv::Sprite&);

  void Load(Vector2);
  void UnloadECS();
private:
  const Map& m_Map;
  const adv::Sprite m_PlayerSprite;
  const adv::Sprite m_TileSprite;
  std::vector<Entity> m_Entities {};
};
