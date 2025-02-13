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
#include "animations/PlayerMovement.h"

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
  
  m_MapLoader = std::make_unique<MapLoader>("res/maps");

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));
  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(TILES_PATH)));
  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(FLAG_PATH)));
  
  // ECS 
  registry.Init();
  InitComponents();
  InitSystems();
  InitLevels();

  Demo();
}

void Game::Run()
{
  m_Running = true;
  m_CurrentLevel->Load();

  while (m_Running && (m_Running = !WindowShouldClose())) {
	m_DT = GetFrameTime();
	ClearBackground(SKYBLUE);
	for (const auto& s : m_UpdateSystems)
	  s->Update(m_DT);

	BeginDrawing();
	  DrawFPS(0, 0);
	  BeginBlendMode(BLEND_ALPHA);
	    BeginMode2D(*m_CurrentLevel->GetCamera().camera);
		
	    for (const auto& s : m_DrawSystems)
	      s->Update(m_DT);
		
	    EndMode2D();
	  EndBlendMode();
	EndDrawing();

	EventManager::Get().Dispatch();

	if (m_PendingNextLevel) 
	  NextLevel();
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

Entity Game::CreatePlayer(adv::Sprite sprite, const adv::Camera& cam, const Vector2 ipos)
{
  Entity e = registry.CreateEntity();
  Vector2 spriteSize = { (float)sprite.GetTexture()->width * .125f, (float)sprite.GetTexture()->height * .125f };

  adv::Animator animator;
  animator.Insert(adv::PlayerState::WALK_LEFT, WALK_LEFT_ANIMATION);
  animator.Insert(adv::PlayerState::WALK_RIGHT, WALK_RIGHT_ANIMATION);
  animator.Insert(adv::PlayerState::JUMP, JUMP_ANIMATION);
  animator.Insert(adv::PlayerState::IDLE, IDLE_ANIMATION);
  animator.ChangeTo(adv::PlayerState::IDLE);

  registry.AddComponent(e, adv::Player());
  registry.AddComponent(e, adv::RigidBody(Vector2Zero(), Vector2Zero(),
										  100.0f, 100.0f, 50.0f, 0.01f, true));
  registry.AddComponent(e, adv::Transform(ipos, spriteSize * .25f, Vector2Zero()));
  registry.AddComponent(e, adv::Collider(spriteSize * .25f, false));
  registry.AddComponent(e, sprite);
  registry.AddComponent(e, cam);
  registry.AddComponent(e, animator);

  std::cout << "Creating player at:\n\t" << ipos << '\n';

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

  std::cout << "Creating tile at:\n\t" << transform <<
	"\n\tfrom tile: " << tile.pos << ", size: " << size << '\n'; 
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
	// renderCollidersSystem,
  };
}

void Game::InitLevels()
{
  adv::Sprite playerSprite(m_Textures.at(0), { 8, 8 }, 0);
  adv::Sprite tileSprite(m_Textures.at(1), { 10, 6 }, 0);
  adv::Sprite flagSprite(m_Textures.at(2), { 5, 1 }, 0);

  m_MapLoader->LoadFile("level1");
  m_MapLoader->LoadFile("level2");
  m_Levels.reserve(2);
  
  m_Levels.emplace_back(m_MapLoader->GetMap("level1"),
						playerSprite,
						tileSprite, 
						flagSprite);
  
  m_Levels.emplace_back(m_MapLoader->GetMap("level2"),
						playerSprite,
						tileSprite, 
						flagSprite);

  m_CurrentLevel = m_Levels.begin();
}

void Game::NextLevel()
{
  m_PendingNextLevel = false;
  
  m_CurrentLevel->Unload();
  
  if ((m_Running = (++m_CurrentLevel != m_Levels.end()))) 
	m_CurrentLevel->Load();
  
  EventManager::Get().Flush(EventType::CheckPoint);
}

void Game::Demo()
{
}
