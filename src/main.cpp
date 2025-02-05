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

Registry registry;

int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");

  Texture2D texture = LoadTexture("res/sprites/old_man.jpeg");
  float sprite_size = (float)texture.width / 8.0f;
  Rectangle source = { 0, 0, sprite_size, sprite_size };
  Rectangle dest = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, sprite_size / 2.0f, sprite_size / 2.0f };

  registry.Init();
  registry.RegisterComponent<Square>();
  registry.RegisterComponent<Gravity>();
  registry.RegisterComponent<RigidBody>();
  registry.RegisterComponent<Transform>();
  
  auto physicsSystem = registry.RegisterSystem<PhysicsSystem>();
  { 
	Signature sign;
	sign.set(registry.GetComponentID<Square>());
	sign.set(registry.GetComponentID<Gravity>());
	sign.set(registry.GetComponentID<RigidBody>());
	sign.set(registry.GetComponentID<Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  auto renderSystem  = registry.RegisterSystem<RenderSystem>();
  {
	Signature sign;
	sign.set(registry.GetComponentID<Square>());
	sign.set(registry.GetComponentID<Transform>());
	registry.SetSystemSignature<PhysicsSystem>(sign);
  }

  std::vector<Entity> entities(MAX_ENTITIES);

  SetTargetFPS(60);

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

	registry.AddComponent(e, Square{{
		  (unsigned char)GetRandomValue(0, 255), 
		  (unsigned char)GetRandomValue(0, 255), 
		  (unsigned char)GetRandomValue(0, 255),
		  255
		}});
  }

  float dt = 0.0f;
  while (!WindowShouldClose()) {
	dt = GetFrameTime();
	ClearBackground(WHITE);
	physicsSystem->Update(dt);

	BeginDrawing();
	renderSystem->Update();
	EndDrawing();
  }
}

