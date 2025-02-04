#include <iostream>
#include <raylib.h>
#include <vector>
#include <ecs/Registry.hpp>
#include "ecs/Components.hpp"
#include "ecs/Systems.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"
#include "ecs/EntityManager.hpp"

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f

float normalize(float v, float t, float n) {return v / t * n;}


int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");

  Texture2D texture = LoadTexture("res/sprites/old_man.jpeg");
  float sprite_size = (float)texture.width / 8.0f;
  Rectangle source = { 0, 0, sprite_size, sprite_size };
  Rectangle dest = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, sprite_size / 2.0f, sprite_size / 2.0f };

  Registry registry;
  registry.Init();
  registry.RegisterComponent<Square>();
  registry.RegisterComponent<Gravity>();
  registry.RegisterComponent<RigidBody>();
  registry.RegisterComponent<Transform>();

  auto physics = registry.RegisterSystem<PhysicsSystem>();

  Signature sig;
  sig.set(registry.GetComponentID<Square>());
  sig.set(registry.GetComponentID<Gravity>());
  sig.set(registry.GetComponentID<RigidBody>());
  sig.set(registry.GetComponentID<Transform>());
  registry.SetSystemSignature<PhysicsSystem>(sig);

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
	BeginDrawing();
	DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);

	/*
	for (Square& s : squares) {
	  DrawRectangleV(s.position, {10, 10}, s.color);
	  s.position.y++;
	}
	*/
	EndDrawing();
  }
}

