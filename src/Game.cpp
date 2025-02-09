#include "Game.h"
#include <iostream>
#include <format>
#include <cstdio>
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
  auto oldman = m_Textures.at(0);
  Entity player = registry.CreateEntity();
  Vector2 pInitialPos = m_Window->GetCenter() + Vector2{ 0.0f, 100.0f };
  adv::RigidBody playerRigidBody(Vector2Zero(), Vector2Zero(),
								 10.0f, .5f, .1f, .5f, true);
  CreateSprite(player, oldman, pInitialPos, playerRigidBody);
  registry.AddComponent(player, adv::Player{});

  auto tiles = m_Textures.at(1);
  Entity floor = registry.CreateEntity();
  Vector2 tilesSize = { tiles->width * .1f, tiles->height / 6.0f };
  Vector2 floorSize = { (float)m_Window->GetWidth(), m_Window->GetHeight() * .25f };
  Rectangle src = { tilesSize.x * 3.0f, tilesSize.y * 4.0f, tilesSize.x, tilesSize.y };
  
  adv::Transform transform(m_Window->GetCenter(), floorSize, Vector2Zero());
  adv::ToBottom(transform, m_Window->GetHeight());
  registry.AddComponent(
						floor,
						adv::RigidBody::CreateStatic(1.0f, 1.0f),
						adv::Sprite(tiles, src),
						transform,
						adv::Collider(floorSize)
						);
}
