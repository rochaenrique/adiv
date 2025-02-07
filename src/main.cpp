#include <iostream>
#include <raylib.h>
#include <vector>
#include <memory>
#include "ecs/systems/Systems.h"
#include "ecs/Registry.h"
#include "ecs/components/Components.h"
#include "ecs/components/Collider.h"
#include "ecs/SystemManager.h"
#include "ecs/EntityManager.h"
#include "ecs/ComponentManager.h"

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f


void createSprite(Entity&, const std::shared_ptr<Texture2D>&, const Vector2, const adv::RigidBody&);
  
Registry registry;

int main()
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");
  registry.Init();

  registry.RegisterComponent<adv::Player>();
  registry.RegisterComponent<adv::Sprite>();
  registry.RegisterComponent<adv::Collider>();
  registry.RegisterComponent<adv::RigidBody>();
  registry.RegisterComponent<adv::Transform>();

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

  
  auto playerUpdateSystem = registry.RegisterSystem<PlayerUpdateSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Player>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PlayerUpdateSystem>(sign);
  }

  auto oldman = std::make_shared<Texture2D>(LoadTexture("res/sprites/old_man.jpeg"));
  
  std::vector<Entity> entities(MAX_ENTITIES);
  for (Entity& e : entities) {
	e = registry.CreateEntity();
	auto r = adv::RigidBody(Vector2Zero(),
							{ (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10) },
							(float)GetRandomValue(1, 15),
							(float)GetRandomValue(0, 5),
							(float)GetRandomValue(0, 5)
							);
	createSprite(e, oldman, { (float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0, SCREEN_WIDTH) }, r);
  }

  SetTargetFPS(60);
  float dt = 0.0f;
  while (!WindowShouldClose()) {
	dt = GetFrameTime();
	ClearBackground(WHITE);
	
	physicsSystem->Update(dt);
	collisionSystem->Update();
	playerUpdateSystem->Update(dt);
	
	BeginDrawing();
	renderSystem->Update();
	//renderCollidersSystem->Update();
	EndDrawing();
  }
}

void createSprite(Entity& e, const std::shared_ptr<Texture2D>& t, const Vector2 ipos, const adv::RigidBody& r)
{
  float sprite_size = (float)t->width / 8.0f;
  
  registry.AddComponent(e, r);
  registry.AddComponent(e, adv::Transform(ipos, {sprite_size / 8.0f, sprite_size / 8.0f}, Vector2Zero()));
  registry.AddComponent(e, adv::Collider({ ipos.x, ipos.y, sprite_size/ 8.0f, sprite_size / 8.0f }));
  registry.AddComponent(e, adv::Sprite(t, { 0, 0, sprite_size, sprite_size }));
}

