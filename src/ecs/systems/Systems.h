#pragma once
#include "ecs/ECS.h"
#include <raylib.h>

class PhysicsSystem : public System
{
public:
  void Update(float);
private:
  static constexpr Vector2 G = { 0.0f, 9.81f };
};

class CollisionSystem : public System
{
public:
  void Update();
};

class RenderSystem : public System
{
public:
  void Update();
};

class RenderCollidersSystem : public System
{
public:
  void Update();
};

class PlayerUpdateSystem : public System
{
public:
  void Update(float);
};
