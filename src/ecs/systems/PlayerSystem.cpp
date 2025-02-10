#include "PlayerSystem.h"
#include <iostream>
#include <raylib.h>
#include <raylib.h>
#include <cmath>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"

#define VERT_MULT  4000.0f
#define HOR_MULT   50.0f

extern Registry registry;

void PlayerSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	if (IsKeyPressed(KEY_R)) {
	  adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	  t.translation = Vector2Zero();
	  t.rotation    = Vector2Zero();
	} else {
	  adv::RigidBody& body = registry.GetComponent<adv::RigidBody>(e);
	  Vector2 step = {};
	  if (std::abs(adv::IsWorldUp(body.GetVelocity())) <= 1.0f) 
		step.y = (float)(IsKeyPressed(KEY_S) - IsKeyPressed(KEY_W)) * VERT_MULT;
	  
	  step.x = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * HOR_MULT;
	  body.ApplyAcc(step);
	}
  }
}
