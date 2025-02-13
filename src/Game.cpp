#include "Game.h"
#include <iostream>
#include <format>
#include <cstdio>
#include "Animation.h"
#include "EventManager.h"
#include "ecs/Systems.h"
#include "ecs/Components.h"
#include "util/Random.h"
#include "util/Helper.h"

#define	SPRITE_DIR  "res/sprites"
#define OLDMAN_PATH SPRITE_DIR"/old_man.png"
#define TILES_PATH  SPRITE_DIR"/tiles.png"
#define FLAG_PATH	SPRITE_DIR"/flag.png"

Registry registry;

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();
  
  EventManager::Get().SetEventCallback([this](Event& e) {
	return OnEvent(e);
  });
  
  m_LevelLoader = std::make_unique<LevelLoader>("res/maps");
  // ECS 
  registry.Init();
  InitComponents();
  InitSystems();
  InitLevels();
}

void Game::Run()
{
  m_Running = true;
  m_LevelLoader->InitLevels();
  
  while (m_Running && (m_Running = !WindowShouldClose())) {
	m_DT = GetFrameTime();
	ClearBackground(SKYBLUE);
	for (const auto& s : m_UpdateSystems)
	  s->Update(m_DT);

	BeginDrawing();
	  DrawFPS(0, 0);
	  BeginBlendMode(BLEND_ALPHA);
	    BeginMode2D(*m_LevelLoader->GetCamera());
	  
	      for (const auto& s : m_DrawSystems)
	        s->Update(m_DT);
		
	    EndMode2D();
	  EndBlendMode();
	EndDrawing();

	EventManager::Get().Dispatch();

	if (m_PendingNextLevel) {
	  m_Running = m_LevelLoader->NextLevel();
	  m_PendingNextLevel = false;
	}
  }
}

bool Game::OnEvent(Event& e)
{
  if (e.IsHandled()) return false;
  
  switch (e.GetType()) {
  case CheckPoint:
	m_PendingNextLevel = true;
	e.SetHandled();
	break;
  }
  return true;
}

Entity Game::CreatePlayer(adv::Sprite sprite, const adv::Camera& cam, const Vector2 ipos, const adv::Animator animator)
{
  Entity e = registry.CreateEntity();
  Vector2 spriteSize = { (float)sprite.GetTexture()->width * .125f, (float)sprite.GetTexture()->height * .125f };

  registry.AddComponent(e, adv::Player());
  registry.AddComponent(e, adv::RigidBody(Vector2Zero(), Vector2Zero(),
										  100.0f, 100.0f, 50.0f, 0.01f, true));
  registry.AddComponent(e, adv::Transform(ipos, spriteSize * .25f, Vector2Zero()));
  registry.AddComponent(e, adv::Collider(spriteSize * .25f, false));
  registry.AddComponent(e, sprite);
  registry.AddComponent(e, cam);
  registry.AddComponent(e, animator);

  return e;
}

Entity Game::CreateTile(adv::Sprite sprite, Tile tile, Vector2 grid, Vector2 size, const adv::Collider& collider)
{
  Entity e = registry.CreateEntity();
  sprite.SetIndex(tile.spriteIndex);
  
  Vector2 pos = { size.x * (tile.pos.x + .5f), 0 };
  adv::Transform transform(pos, size, Vector2Zero());
  adv::ToBottom(transform, size.y * (grid.y - tile.pos.y));

  registry.AddComponent(e, adv::RigidBody::CreateStatic(10.0f, 5.0f));
  registry.AddComponent(e, sprite);
  registry.AddComponent(e, transform);
  registry.AddComponent(e, collider);

  // std::cout << "Creating tile at:\n\t" << transform <<
  // 	"\n\tfrom tile: " << tile.pos << ", size: " << size << '\n'; 
  return e;
}

void Game::InitComponents() const
{
  registry.RegisterComponent<adv::Player>();
  registry.RegisterComponent<adv::Sprite>();
  registry.RegisterComponent<adv::Collider>();
  registry.RegisterComponent<adv::RigidBody>();
  registry.RegisterComponent<adv::Transform>();
  registry.RegisterComponent<adv::Camera>();
  registry.RegisterComponent<adv::Animator>();
  registry.RegisterComponent<adv::SpriteAnimation>();
}

void Game::InitSystems()
{
  auto physicsSystem = registry.RegisterSystem<PhysicsSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }
  
  auto collisionSystem = registry.RegisterSystem<CollisionSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<adv::Collider>());
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<CollisionSystem>(sign);
  }
  
  auto playerSystem = registry.RegisterSystem<PlayerSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Player>());
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Animator>());

	registry.SetSystemSignature<PlayerSystem>(sign);
  }

  auto renderSystem = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<RenderSystem>(sign);
  }
  
  auto renderCollidersSystem = registry.RegisterSystem<RenderCollidersSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Collider>());
	registry.SetSystemSignature<RenderCollidersSystem>(sign);
  }

  auto cameraSystem = registry.RegisterSystem<CameraSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Camera>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<CameraSystem>(sign);
  }
  
  auto animatorSystem = registry.RegisterSystem<AnimatorSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Animator>());
	registry.SetSystemSignature<AnimatorSystem>(sign);
  }
  
  auto spriteAnimationSystem = registry.RegisterSystem<SpriteAnimationSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::SpriteAnimation>());
	sign.set(registry.GetComponentID<adv::Sprite>());
	registry.SetSystemSignature<SpriteAnimationSystem>(sign);
  }

  m_UpdateSystems = {
	collisionSystem,
	playerSystem,
	physicsSystem,
	cameraSystem,
	animatorSystem,
	spriteAnimationSystem,
  };

  m_DrawSystems = {
	renderSystem,
	renderCollidersSystem,
  };
}

void Game::InitLevels()
{
  m_LevelLoader->LoadFile("level1");
  // m_LevelLoader->LoadFile("level2");
}

void Game::Demo()
{
}
