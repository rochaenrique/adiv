#pragma once

#include <memory>
#include <raylib.h>
#include "Window.h"
#include "ecs/ECS.h"
#include "ecs/systems/Systems.h"
#include "ecs/components/Components.h"

class Game
{
public:
  Game(const WindowOptions& wopt = WindowOptions());
  void Run();
private:
  float m_DT = 0.0f;
  std::unique_ptr<Window> m_Window;
  std::array<Entity, MAX_ENTITIES> m_Entities {};
  std::vector<std::shared_ptr<Texture2D>> m_Textures {};
  
  std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
  std::shared_ptr<CollisionSystem> m_CollisionSystem;
  std::shared_ptr<RenderSystem> m_RenderSystem;
  std::shared_ptr<RenderCollidersSystem> m_RenderCollidersSystem;
  std::shared_ptr<PlayerUpdateSystem> m_PlayerUpdateSystem;
  
  void InitComponents();
  void InitSystems();
  void CreateEntities();
  void CreateSprite(Entity&, const std::shared_ptr<Texture2D>&, const Vector2, const adv::RigidBody&);
};
