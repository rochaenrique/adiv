#include "Game.h"
#include <iostream>
#include <format>
#include <cstdio>
#include "Animation.h"
#include "ecs/Systems.h"
#include "ecs/Components.h"
#include "util/Random.h"
#include "util/Helper.h"

#define SPRITE_DIR  "res/sprites"
#define OLDMAN_PATH SPRITE_DIR"/old_man.png"
#define TILES_PATH  SPRITE_DIR"/tiles.png"

Registry registry;
float playerIndex;

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();

  m_MapLoader = std::make_unique<MapLoader>("res/maps");
  m_MapLoader->LoadFile("level1");

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));
  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(TILES_PATH)));
  
  // ECS 
  registry.Init();
  InitComponents();
  InitSystems();


  Demo(); // TODO REMOVE
}

void Game::Run()
{
  std::cout << "STARTING\n";
  Vector2 center = m_Window->GetCenter();
  while (!WindowShouldClose()) {
	m_DT = GetFrameTime();
	ClearBackground(SKYBLUE);

	for (const auto& s : m_UpdateSystems)
	  s->Update(m_DT);
	
	BeginDrawing();
	  DrawFPS(0, 0);
	  
	  // TEMPORARY
	  DrawText(TextFormat("Player Animation: %f", playerIndex),
			   center.x, center.y, 20, WHITE);
	  BeginBlendMode(BLEND_ALPHA);
	  for (const auto& s : m_DrawSystems)
		s->Update(m_DT);
	  EndBlendMode();
	EndDrawing();
  }
}

void Game::InitComponents() const
{
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
	registry.SetSystemSignature<RenderSystem>(sign);
  }
  
  auto renderCollidersSystem  = registry.RegisterSystem<RenderCollidersSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Collider>());
	registry.SetSystemSignature<RenderCollidersSystem>(sign);
  }

  playerSystem->m_Animation = adv::Animation({
	  {
		.from	  = 100.0f,
		.to		  = 0.0f,
		.duration = 10.0f 
	  },
	  {
		.from	  = 25.0f,
		.to		  = 50.0f,
		.duration = 50.0f 
	  },
	}, true);
  
  m_UpdateSystems = {
	playerSystem,
	collisionSystem,
	physicsSystem,
  };

  m_DrawSystems = {
	renderSystem,
	// renderCollidersSystem,
  };
}

void Game::CreatePlayer(Entity& e, const std::shared_ptr<Texture2D>&t, const Vector2 ipos, const adv::RigidBody& r)
{
  float spriteWidth = (float)t->width * .125f;
  Vector2 spriteSize = { spriteWidth, spriteWidth };
  Vector2 realSize = spriteSize * .25f;
  adv::Player playerComp;
  registry.AddComponent(e, playerComp);
	
  adv::Collider collider(realSize);
  collider.SetCollisionCallback([](const adv::Collision& col, float) {
	auto pa = registry.RiskyGetComponent<adv::Player>(col.a);
	auto pb = registry.RiskyGetComponent<adv::Player>(col.b);
	
	if (pa) pa->SetJumping(false);
	else if (pb) pb->SetJumping(false);
  });
  
  registry.AddComponent(e, r);
  registry.AddComponent(e, adv::Transform(ipos, realSize, Vector2Zero()));
  registry.AddComponent(e, collider);
  registry.AddComponent(e, adv::Sprite(t, spriteSize, 0));
}

void Game::Demo()
{
  // player
  Map& map = m_MapLoader->GetMap("level1");
  
  auto oldman = m_Textures.at(0);
  Entity player = registry.CreateEntity();
  adv::RigidBody playerRigidBody(Vector2Zero(), Vector2Zero(),
								 100.0f, 100.0f, 50.0f, 0.01f, true);
  CreatePlayer(player, oldman, map.playerInitialPos, playerRigidBody);

  // tiles
  auto tiles = m_Textures.at(1);
  Vector2 tileTexSize = { tiles->width * .1f, tiles->height / 6.0f };
  Vector2 tileSize = { (float)m_Window->GetWidth() / map.grid.x, m_Window->GetHeight() * map.grid.y };
  
  for (const std::vector<std::pair<size_t, size_t>>& row : map.tiles) {
	for (const std::pair<size_t, size_t>& pair : row) {
	  Entity e = registry.CreateEntity();
	  Vector2 pos = {tileSize.x * (i + .5f), 0};
	  adv::Transform transform(pos, tileSize, Vector2Zero());
	  adv::ToBottom(transform, m_Window->GetHeight());
	
	  std::cout << "Adding tile at (" << transform.translation.x << ", " <<
		transform.translation.y << ")\n";
	
	  registry.AddComponent(e, adv::RigidBody::CreateStatic(10.0f, 5.0f));
	  registry.AddComponent(e, adv::Sprite(tiles, tileTexSize, 43));
	  registry.AddComponent(e, transform);
	  registry.AddComponent(e, adv::Collider(tileSize));
	
	  m_Entities.push_back(e);
	}
  }

  playerIndex = 0;
}
