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

float normalize(float v, float t, float n) {return v / t * n;}
void registerComponents();

Registry registry;

int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");
  registry.Init();

  auto physicsSystem = registry.RegisterSystem<PhysicsSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<Gravity>());
	sign.set(registry.GetComponentID<RigidBody>());
	sign.set(registry.GetComponentID<Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  auto renderSystem  = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<Sprite>());
	sign.set(registry.GetComponentID<Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  std::vector<Entity> entities(MAX_ENTITIES);
  for (Entity& e : entities) {
	e = registry.CreateEntity();

	registry.AddComponent(e, Gravity{{0.0f, (float)GetRandomValue(5, 15)}});
	registry.AddComponent(e, RigidBody{
		{0, 0}, {0, 0}
	  });

	registry.AddComponent(e, Transform{
		.translation = {(float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0, SCREEN_WIDTH), 0},
		.rotation = {0, 0, 0, 0},
		.scale = {10, 10, 10},
	  });

	registry.AddComponent(e, Sprite{0});
  }

  auto oldman = std::make_shared<Texture2D>(LoadTexture("res/sprites/old_man.jpeg"));
  float sprite_size = (float)texture.width / 8.0f;
  Rectangle source = { 0, 0, sprite_size, sprite_size };
  Rectangle dest = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, sprite_size / 2.0f, sprite_size / 2.0f };

  Entity player = registry.CreateEntity();
  registry.AddComponent(player, adv::Gravity{{0.0f, (float)GetRandomValue(5, 15)}});
  registry.AddComponent(player, adv::RigidBody{
	  {0, 0}, {0, 0}
	});

  registry.AddComponent(e, adv::Transform{
	  .translation = {SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f},
	  .scale = {10, 10},
	});

  registry.AddComponent(e, Sprite{0});

  SetTargetFPS(60);
  float dt = 0.0f;
  while (!WindowShouldClose()) {
	dt = GetFrameTime();
	ClearBackground(WHITE);
	physicsSystem->Update(dt);
	
	DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
	
	BeginDrawing();
	renderSystem->Update();
	EndDrawing();
  }
}

void registerComponents()
{
  registry.RegisterComponent<Sprite>();
  registry.RegisterComponent<Gravity>();
  registry.RegisterComponent<RigidBody>();
  registry.RegisterComponent<Transform>();
}
