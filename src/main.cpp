#include <iostream>
#include <raylib.h>
#include <vector>
#include "ecs/systems/Systems.h"
#include "ecs/Registry.h"
#include "ecs/components/Components.h"
#include "ecs/SystemManager.h"
#include "ecs/EntityManager.h"
#include "ecs/ComponentManager.h"

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f

void createSprite(Entity&, std::shared_ptr<Texture2D>&);
  
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
  Entity p = registry.CreateEntity();
  createSprite(p, oldman);
  registry.AddComponent(p, adv::Player{});

  Entity q = registry.CreateEntity();
  createSprite(q, oldman);

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
	renderCollidersSystem->Update();
	EndDrawing();
  }
}

void createSprite(Entity& e, std::shared_ptr<Texture2D>& t)
{
  registry.AddComponent(e, adv::RigidBody{
	  .force = {0, 0},
	  .velocity = {0, 0},
	  .mass = 1.0f
	});

  Vector2 pos = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  float sprite_size = (float)t->width / 8.0f;
  registry.AddComponent(e, adv::Transform{
	  .translation = pos,
	  .scale = {sprite_size, sprite_size},
	  .rotation = {0, 0}
	});
  
  registry.AddComponent(e, adv::Collider({
		pos.x, pos.y, sprite_size / 2.0f, sprite_size 
	  }));
  Rectangle source = { 0, 0, sprite_size, sprite_size };
  registry.AddComponent(e, adv::Sprite{
	  .texture = t,
	  .source = source,
	});
}

