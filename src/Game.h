#pragma once
#include <memory>
#include <raylib.h>
#include "Window.h"
#include "Level.h"
#include "LevelLoader.h"
#include "ecs/ECS.h"
#include "ecs/Registry.h"
#include "ecs/Systems.h"
#include "ecs/Components.h"
#include "Event.h"

class Game
{
public:
  Game(const WindowOptions& wopt = WindowOptions());
  void Run();

  bool OnEvent(Event&);

  static Entity CreatePlayer(adv::Sprite, const adv::Camera&, const Vector2);
  static Entity CreateTile(adv::Sprite, Tile, Vector2, Vector2, const adv::Collider&);

private:
  float m_DT = 0.0f;
  bool m_Running = false;
  
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<LevelLoader> m_LevelLoader;
  
  std::vector<std::shared_ptr<System>> m_UpdateSystems;
  std::vector<std::shared_ptr<System>> m_DrawSystems;

  bool m_PendingNextLevel = false;
  
  void InitComponents() const;
  void InitSystems();
  void InitLevels();
  
  void Demo(); // TODO: Remove
};
