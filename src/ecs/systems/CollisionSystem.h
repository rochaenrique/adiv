#pragma once
#include "ecs/ECS.h"
#include "ecs/components/Collider.h"

class CollisionSystem : public System
{
public:
  void Update(float) override;
  static void ImpulseSolver(std::vector<adv::Collision>&);
  static void PositionSolver(std::vector<adv::Collision>&);
};


