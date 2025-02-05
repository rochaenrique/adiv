#pragma once
#include "ECS.h"
#include <raylib.h>

class PhysicsSystem : public System
{
public:
  void Update(float dt);
};

class RenderSystem : public System
{
public:
  void Update();
};
