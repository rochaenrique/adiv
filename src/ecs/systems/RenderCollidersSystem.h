#pragma once
#include "ecs/ECS.h"
#include "ecs/components/Collider.h"

class RenderCollidersSystem : public System
{
public:
  void Update(float);
};

