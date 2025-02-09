#include "Game.h"
#include <iostream>
#include "ecs/components/Collider.h"

#define OLDMAN_PATH "res/sprites/old_man.jpeg"

Registry registry;

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));

  // ECS 
  registry.Init();
  InitComponents();
  InitSystems();
  CreateEntities();
}

void Game::Run()
{
  std::cout << "STARTING\n";
  while (!WindowShouldClose()) {
	m_DT = GetFrameTime();
	ClearBackground(WHITE);

	for (const auto& s : m_UpdateSystems)
	  s->Update(m_DT);
	
	BeginDrawing();
	{
	  for (const auto& s : m_DrawSystems)
		s->Update(m_DT);
	}
	EndDrawing();
  }
}

void Game::InitComponents() const
{
  registry.RegisterComponent<
	adv::Player,
	adv::Sprite,
	adv::Collider,
	adv::RigidBody,
	adv::Transform
   >();
}

void Game::InitSystems()
{
  m_UpdateSystems = { 
	registry.CreateSystem<
	  PhysicsSystem,
	  adv::RigidBody,
	  adv::Transform
	>(),
	registry.CreateSystem<
	  CollisionSystem,
	  adv::Collider,
	  adv::RigidBody,
	  adv::Transform
	>(),
	registry.CreateSystem<
	  PlayerUpdateSystem,
	  adv::Sprite,
	  adv::Player,
	  adv::Transform
	>()
  };

  m_DrawSystems = {
	registry.CreateSystem<
	  RenderSystem,
	  adv::Sprite,
	  adv::Transform
	>(),
	registry.CreateSystem<
	  RenderCollidersSystem,
	  adv::Collider
	>(),
  };
}

void Game::CreateEntities()
{
  unsigned width = m_Window->GetWidth();
  unsigned height = m_Window->GetHeight();
  auto oldman = m_Textures.at(0);
  
  for (Entity& e : m_Entities) {
	e = registry.CreateEntity();
	auto r = adv::RigidBody({ (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10) },
							{ (float)GetRandomValue(-20, 20), (float)GetRandomValue(-30, 30) },
							(float)GetRandomValue(1, 15),
							(float)GetRandomValue(0, 5),
							(float)GetRandomValue(0, 5)
							);
	CreateSprite(e, oldman, { (float)GetRandomValue(0, width), (float)GetRandomValue(0, height) }, r);
  }
}

void Game::CreateSprite(Entity& e, const std::shared_ptr<Texture2D>&t, const Vector2 ipos, const adv::RigidBody& r)
{
  float sprite_size = (float)t->width / 8.0f;
  
  registry.AddComponent(e, r);
  registry.AddComponent(e, adv::Transform(ipos, {sprite_size / 8.0f, sprite_size / 8.0f}, Vector2Zero()));
  registry.AddComponent(e, adv::Collider({ ipos.x, ipos.y, sprite_size/ 8.0f, sprite_size / 8.0f }));
  registry.AddComponent(e, adv::Sprite(t, { 0, 0, sprite_size, sprite_size }));
}
