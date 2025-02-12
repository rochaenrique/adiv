#pragma once
#include "ecs/ECS.h"
#include <raylib.h>
#include "Animation.h"

#define V_MV_TOLERANCE  2.0f
#define H_MV_TOLERANCE  1.0f
#define V_MULT          2500.0f
#define H_NO_JUMP_MULT  300.0f
#define H_JUMP_MULT     H_NO_JUMP_MULT * .1f

class PlayerSystem : public System
{
public:
  void Update(float) override;
  Vector2 CalculateStep(Vector2, Vector2);
};
