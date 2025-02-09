#pragma once
#include "ecs/ECS.h"
#include <raylib.h>

class PhysicsSystem : public System
{
public:
  void Update(float) override;
private:
  static constexpr Vector2 G = { 0.0f, 6.0f };
};

class CollisionSystem : public System
{
public:
  void Update(float) override;
};

class PlayerUpdateSystem : public System
{
public:
  void Update(float) override;
};

class RenderSystem : public System
{
public:
  void Update(float);
};

class RenderCollidersSystem : public System
{
public:
  void Update(float);
};
