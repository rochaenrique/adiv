#pragma once
#include "ecs/ECS.h"
#include <raylib.h>

class PhysicsSystem : public System
{
public:
  void Update(float);
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
