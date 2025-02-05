#include <iostream>
#include <raylib.h>
#include <vector>
#include "ecs/Systems.h"
#include "ecs/Registry.h"
#include "ecs/Components.h"
#include "ecs/SystemManager.h"
#include "ecs/EntityManager.h"
#include "ecs/ComponentManager.h"

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f

void registerComponents();

Registry registry;

int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");
  registry.Init();

  registry.RegisterComponent<adv::Player>();
  registry.RegisterComponent<adv::Sprite>();
  registry.RegisterComponent<adv::Gravity>();
  registry.RegisterComponent<adv::RigidBody>();
  registry.RegisterComponent<adv::Transform>();

  auto physicsSystem = registry.RegisterSystem<PhysicsSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<adv::Gravity>());
	sign.set(registry.GetComponentID<adv::RigidBody>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  auto renderSystem  = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }
  
  auto playerUpdateSystem  = registry.RegisterSystem<PlayerUpdateSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<adv::Sprite>());
	sign.set(registry.GetComponentID<adv::Player>());
	sign.set(registry.GetComponentID<adv::Transform>());
	registry.SetSystemSignature<PlayerUpdateSystem>(sign);
  }


  auto oldman = std::make_shared<Texture2D>(LoadTexture("res/sprites/old_man.jpeg"));
  Entity player = registry.CreateEntity();
  {
	float sprite_size = (float)oldman->width / 8.0f;
	Rectangle source = { 0, 0, sprite_size, sprite_size };
	registry.AddComponent(player, adv::Gravity{
		.force = {0, 0}
	  });
	registry.AddComponent(player, adv::RigidBody{
		.acceleration = {0, 0},
		.velocity = {0, 0}
	  });
	registry.AddComponent(player, adv::Transform{
		.translation = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},
		.scale = {sprite_size, sprite_size},
		.rotation = {0, 0}
	  });
	registry.AddComponent(player, adv::Sprite{
		.texture = oldman,
		.source = source,
	  });
	registry.AddComponent(player, adv::Player{});
  }
  
  SetTargetFPS(60);
  float dt = 0.0f;
  while (!WindowShouldClose()) {
	dt = GetFrameTime();
	ClearBackground(WHITE);
	physicsSystem->Update(dt);
	playerUpdateSystem->Update(dt);
	
	BeginDrawing();
	renderSystem->Update();
	EndDrawing();
  }
}

void registerComponents()
{
}
