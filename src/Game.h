#pragma once
#include <memory>
#include <raylib.h>
#include "Window.h"
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
  // std::array<Entity, MAX_ENTITIES> m_Entities {};
  std::vector<Entity> m_Entities {};
  std::vector<std::shared_ptr<Texture2D>> m_Textures {};

  std::vector<std::shared_ptr<System>> m_UpdateSystems;
  std::vector<std::shared_ptr<System>> m_DrawSystems;
  
  void InitComponents() const;
  void InitSystems();
  void CreateEntities();
  static void CreateSprite(Entity&, const std::shared_ptr<Texture2D>&, const Vector2, const adv::RigidBody&);
  
  void Demo(); // TODO: Remove
};
