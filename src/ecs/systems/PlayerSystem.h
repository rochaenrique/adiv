#pragma once
#include "ecs/ECS.h"

class PlayerUpdateSystem : public System
{
public:
  void Update(float) override;
};
