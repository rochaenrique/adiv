#pragma once
#include "ecs/ECS.h"

class AnimatorSystem : public System
{
public:
  void Update(float) override;
};
