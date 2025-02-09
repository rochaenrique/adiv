#pragma once
#include <raylib.h>
#include "ecs/ECS.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"


class PhysicsSystem : public System
{
public:
  void Update(float) override;
  void StepBody(adv::RigidBody&, adv::Transform&, float);

private:
  static constexpr Vector2 G = { 0.0f, 6.0f };
};

class CollisionSystem : public System
{
public:
  void Update(float) override;
};

class PlayerUpdateSystem : public System
{
public:
  void Update(float) override;
};

class RenderSystem : public System
{
public:
  void Update(float);
};

class RenderCollidersSystem : public System
{
public:
  void Update(float);
};
