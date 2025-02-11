#pragma once
#include <memory>
#include <raylib.h>
#include "Window.h"
#include "MapLoader.h"
#include "ecs/ECS.h"
#include "ecs/Registry.h"
#include "ecs/Systems.h"
#include "ecs/Components.h"

class Game
{
public:
  Game(const WindowOptions& wopt = WindowOptions());
  void Run();
private:
  float m_DT = 0.0f;
  
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<MapLoader> m_MapLoader;

  std::vector<Entity> m_Entities {};
  std::vector<std::shared_ptr<Texture2D>> m_Textures {};

  std::vector<std::shared_ptr<System>> m_UpdateSystems;
  std::vector<std::shared_ptr<System>> m_DrawSystems;
  
  void InitComponents() const;
  void InitSystems();
  void CreateEntities();
  void LoadMap(const Map& map);
  static void CreatePlayer(Entity&, const std::shared_ptr<Texture2D>&, const Vector2, const adv::RigidBody&);
  
  void Demo(); // TODO: Remove
};
