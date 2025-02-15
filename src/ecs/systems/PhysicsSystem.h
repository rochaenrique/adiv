#pragma once
#include "ecs/ECS.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"

class PhysicsSystem : public System
{
public:
  void Update(float) override;
  static void StepBody(adv::RigidBody&, adv::Transform&, float);
private:
  static constexpr Vector2 G = { 0.0f, 3 * 9.81f };
};
