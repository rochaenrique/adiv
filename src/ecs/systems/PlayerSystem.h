#pragma once
#include "ecs/ECS.h"

class PlayerSystem : public System
{
public:
  void Update(float) override;
};
