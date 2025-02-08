#include "Game.h"
#include "ecs/Registry.h"
#include "ecs/components/Collider.h"

#define OLDMAN_PATH "res/sprites/old_man.jpeg"

Registry registry;

Game::Game(const WindowOptions& wopt)
{
  m_Window = std::unique_ptr<Window>(Window::Create(wopt));
  m_Window->Init();

  m_Textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(OLDMAN_PATH)));

  InitComponents();
  InitSystems();
  CreateEntities();
}

void Game::Run()
{
  while (!WindowShouldClose()) {
	m_DT = GetFrameTime();
	ClearBackground(WHITE);
	
	m_PhysicsSystem->Update(m_DT);
	m_CollisionSystem->Update(m_DT);
	m_PlayerUpdateSystem->Update(m_DT);

	BeginDrawing();
	{
	  m_RenderSystem->Update(m_DT);
	  //m_RenderCollidersSystem->Update(m_DT);
	}
	EndDrawing();
  }
}

void Game::InitComponents()
{
  registry.Init();
  registry.RegisterComponent<adv::Player>();
  registry.RegisterComponent<adv::Sprite>();
  registry.RegisterComponent<adv::Collider>();
  registry.RegisterComponent<adv::RigidBody>();
  registry.RegisterComponent<adv::Transform>();
}

void Game::InitSystems()
{
  m_PhysicsSystem = registry.RegisterSystem<PhysicsSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  m_CollisionSystem = registry.RegisterSystem<CollisionSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<adv::Collider>());
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<CollisionSystem>(sign);
  }

  m_RenderSystem = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<RenderSystem>(sign);
  }

  m_RenderCollidersSystem = registry.RegisterSystem<RenderCollidersSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Collider>());
	registry.SetSystemSignature<RenderCollidersSystem>(sign);
  }

  
  m_PlayerUpdateSystem = registry.RegisterSystem<PlayerUpdateSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Player>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PlayerUpdateSystem>(sign);
  }
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
