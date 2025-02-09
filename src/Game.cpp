#include "Game.h"
#include <iostream>
#include <format>
#include <cstdio>
#include "ecs/Systems.h"
#include "ecs/Components.h"
#include "util/Random.h"
#include "util/Helper.h"

#define SPRITE_DIR  "res/sprites"
#define OLDMAN_PATH SPRITE_DIR"/old_man.jpeg"
#define TILES_PATH  SPRITE_DIR"/tiles.png"

Registry registry;

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));
  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(TILES_PATH)));
  // ECS 
  registry.Init();
  InitComponents();
  InitSystems();
  //CreateEntities();

  // TODO REMOVE
  Demo();
}

void Game::Run()
{
  std::cout << "STARTING\n";
  while (!WindowShouldClose()) {
	m_DT = GetFrameTime();
	ClearBackground(SKYBLUE);

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
  // registry.RegisterComponent<
  // 	adv::Player,
  // 	adv::Sprite,
  // 	adv::Collider,
  // 	adv::RigidBody,
  // 	adv::Transform
  //  >();

  registry.RegisterComponent<adv::Player>();
  registry.RegisterComponent<adv::Sprite>();
  registry.RegisterComponent<adv::Collider>();
  registry.RegisterComponent<adv::RigidBody>();
  registry.RegisterComponent<adv::Transform>();
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
  
  auto playerSystem  = registry.RegisterSystem<PlayerSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Player>());
	sign.set(registry.GetComponentID<adv::RigidBody>());
	registry.SetSystemSignature<PlayerSystem>(sign);
  }

  auto renderSystem  = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  m_UpdateSystems = {
	physicsSystem,
	collisionSystem,
	playerSystem,
  };

  m_DrawSystems = {
	renderSystem,
  };

  // m_UpdateSystems = { 
  // 	registry.CreateSystem<
  // 	  PhysicsSystem,
  // 	  adv::RigidBody,
  // 	  adv::Transform
  // 	>(),
  // 	registry.CreateSystem<
  // 	  CollisionSystem,
  // 	  adv::Collider,
  // 	  adv::RigidBody,
  // 	  adv::Transform
  // 	>(),
  // 	registry.CreateSystem<
  // 	  PlayerSystem,
  // 	  adv::Sprite,
  // 	  adv::Player,
  // 	  adv::RigidBody,
  // 	>()
  // };
  // m_DrawSystems = {
  // 	registry.CreateSystem<
  // 	  RenderSystem,
  // 	  adv::Sprite,
  // 	  adv::Transform
  // 	>(),
  // 	// registry.CreateSystem<
  // 	//   RenderCollidersSystem,
  // 	//   adv::Collider
  // 	// >(),
  // };
}

void Game::CreateEntities()
{
  unsigned width = m_Window->GetWidth();
  unsigned height = m_Window->GetHeight();
  auto oldman = m_Textures.at(0);
  
  for (Entity& e : m_Entities) {
	e = registry.CreateEntity();
	CreateSprite(e, oldman, adv::RandomVector2(width, height), adv::RandomRigidBody());
  }
}

void Game::CreateSprite(Entity& e, const std::shared_ptr<Texture2D>&t, const Vector2 ipos, const adv::RigidBody& r)
{
  float spriteWidth = (float)t->width * .125f;
  Vector2 spriteSize = { spriteWidth, spriteWidth };
  Vector2 realSize = spriteSize * .25f;
  
  registry.AddComponent(e, r);
  registry.AddComponent(e, adv::Transform(ipos, realSize, Vector2Zero()));
  registry.AddComponent(e, adv::Collider(realSize));
  registry.AddComponent(e, adv::Sprite(t, { 0, 0, spriteSize.x, spriteSize.y }));
}

void Game::Demo()
{
  // player
  auto oldman = m_Textures.at(0);
  Entity player = registry.CreateEntity();
  Vector2 pInitialPos = m_Window->GetCenter() + Vector2{ 0.0f, 100.0f };
  adv::RigidBody playerRigidBody(Vector2Zero(), Vector2Zero(),
								 90.0f, .5f, 1.0f, 1.0f, true);
  CreateSprite(player, oldman, pInitialPos, playerRigidBody);
  registry.AddComponent(player, adv::Player{});

  // second, for testing
  Entity second = registry.CreateEntity();
  Vector2 secondInitialPos = pInitialPos + Vector2{ 0.0f, 100.0f };
  adv::RigidBody secondRigidBody = playerRigidBody;
  secondRigidBody.AddForce({ 0.0f, -100.0f });
  CreateSprite(second, oldman, secondInitialPos, secondRigidBody);

  // tiles
  auto tiles = m_Textures.at(1);
  Vector2 tileTexSize = { tiles->width * .1f, tiles->height / 6.0f };
  Rectangle src = { tileTexSize.x * 3.0f, tileTexSize.y * 4.0f, tileTexSize.x, tileTexSize.y };

  const int tileAmount = 10;
  m_Entities.resize(tileAmount);
  
  Vector2 tileSize = { (float)m_Window->GetWidth() / tileAmount, m_Window->GetHeight() * .10f };
  
  for (size_t i = 0; i < tileAmount; i++) {
	m_Entities[i] = registry.CreateEntity();
	Vector2 pos = {tileSize.x * (i + .5f), 0};
	adv::Transform transform(pos, tileSize, Vector2Zero());
	adv::ToBottom(transform, m_Window->GetHeight());
	
	std::cout << "Adding tile at (" << transform.translation.x << ", " <<
	  transform.translation.y << ")\n";
	
	registry.AddComponent(m_Entities[i], adv::RigidBody::CreateStatic(10.0f, 5.0f));
	registry.AddComponent(m_Entities[i], adv::Sprite(tiles, src));
	registry.AddComponent(m_Entities[i], transform);
	registry.AddComponent(m_Entities[i], adv::Collider(tileSize));
  }
}
