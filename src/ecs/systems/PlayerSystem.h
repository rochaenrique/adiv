#pragma once
#include <raylib.h>
#include "ecs/ECS.h"
#include "ecs/components/Player.h"

#define V_MV_TOLERANCE  8.0f
#define V_MULT          1500.0f
#define H_MULT          500.0f

class PlayerSystem : public System
{
public:
  void Update(float) override;
  Vector2 CalculateStep(Vector2, Vector2, bool);
};
