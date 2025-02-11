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

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();
  m_MapLoader = std::make_unique<MapLoader>("res/maps");

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));
  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(TILES_PATH)));
  
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
  m_CurrentLevel->Load(m_Window->GetSize());
  
  while (m_Running && (m_Running = !WindowShouldClose())) {
	m_DT = GetFrameTime();
	ClearBackground(SKYBLUE);

	for (const auto& s : m_UpdateSystems)
	  s->Update(m_DT);

	if (IsKeyPressed(KEY_L)) 
	  NextLevel();
	
	BeginDrawing();
	  DrawFPS(0, 0);
	  BeginBlendMode(BLEND_ALPHA);
	  for (const auto& s : m_DrawSystems)
		s->Update(m_DT);
	  EndBlendMode();
	EndDrawing();
  }
}


Entity Game::CreatePlayer(adv::Sprite sprite, const Vector2 ipos)
{
  Entity e = registry.CreateEntity();
  Vector2 spriteSize = { (float)sprite.GetTexture()->width * .125f, (float)sprite.GetTexture()->height * .125f };
  
  registry.AddComponent(e, adv::Player());
  registry.AddComponent(e, adv::RigidBody(Vector2Zero(), Vector2Zero(),
										  100.0f, 100.0f, 50.0f, 0.01f, true));
  registry.AddComponent(e, adv::Transform(ipos, spriteSize * .25f, Vector2Zero()));
  registry.AddComponent(e, adv::Collider(spriteSize * .25f));
  registry.AddComponent(e, sprite);
  
  return e;
}

Entity Game::CreateTile(adv::Sprite sprite, std::pair<size_t, size_t> tile, Vector2 size, size_t row, size_t nrows)
{
  Entity e = registry.CreateEntity();
  sprite.SetIndex(tile.first);
  
  Vector2 pos = { size.x * (tile.second + .5f), 0 };
  adv::Transform transform(pos, size, Vector2Zero());
  adv::ToBottom(transform, size.y * (nrows - row));

  registry.AddComponent(e, adv::RigidBody::CreateStatic(10.0f, 5.0f));
  registry.AddComponent(e, sprite);
  registry.AddComponent(e, transform);
  registry.AddComponent(e, adv::Collider(size));
  return e;
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

void Game::InitLevels()
{
  adv::Sprite pSprite(m_Textures.at(0), { 8, 8 }, 0);
  adv::Sprite tSprite(m_Textures.at(1), { 10, 6 }, 0);

  m_MapLoader->LoadFile("level1");
  m_MapLoader->LoadFile("level2");
  m_Levels.reserve(2);
  
  m_Levels.emplace_back(m_MapLoader->GetMap("level1"),
						pSprite,
						tSprite);
  
  m_Levels.emplace_back(m_MapLoader->GetMap("level2"),
						pSprite,
						tSprite);

  m_CurrentLevel = m_Levels.begin();
}

void Game::NextLevel()
{
  std::cout << "Loading next level\n";
  m_CurrentLevel->UnloadECS();
  
  m_CurrentLevel++;
  if ((m_Running = (m_CurrentLevel != m_Levels.end()))) 
	m_CurrentLevel->Load(m_Window->GetSize());
}

void Game::Demo()
{
}
