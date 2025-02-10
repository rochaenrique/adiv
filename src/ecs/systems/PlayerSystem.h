#pragma once
#include "ecs/ECS.h"
#include <raylib.h>
#include "Animation.h"

class PlayerSystem : public System
{
public:
  void Update(float) override;
  adv::Animation m_Animation {};
};
