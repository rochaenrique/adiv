#pragma once
#include "ECS.h"
#include <raylib.h>

class PhysicsSystem : public System
{
public:
  void Update(float);
};

class RenderSystem : public System
{
public:
  void Update();
};

class PlayerUpdateSystem : public System
{
public:
  void Update(float);
};
