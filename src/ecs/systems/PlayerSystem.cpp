#include "PlayerSystem.h"
#include <iostream>
#include <raylib.h>
#include "util/Helper.h"
#include <cmath>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"

#define V_MV_TOLERANCE 2.0f
#define V_MULT         2500.0f
#define H_NO_JMP_MULT  300.0f
#define H_JMP_SLOW     .1f

extern Registry registry;

void PlayerSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	if (IsKeyPressed(KEY_R)) { // TODO: TEMPORARY FOR TESTING
	  adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	  t.translation = Vector2Zero();
	  t.rotation    = Vector2Zero();
	} else {
	  adv::RigidBody& body = registry.GetComponent<adv::RigidBody>(e);
	  adv::Player& player = registry.GetComponent<adv::Player>(e);
	  
	  Vector2 step = {};
	  step.x = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * H_NO_JMP_MULT;
	  
	  if (std::abs(adv::IsWorldUp(body.GetVelocity())) <= V_MV_TOLERANCE && !player.IsJumping()) {
		step.y = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * V_MULT;
		player.SetJumping(step.y < 0);
		
	  }	else if (player.IsJumping())
		step.x *= H_JMP_SLOW;
		
	  body.ApplyAcc(step);
	}
  }
}
