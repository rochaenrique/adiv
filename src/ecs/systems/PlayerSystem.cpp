#include "PlayerSystem.h"
#include <iostream>
#include "util/Helper.h"
#include <cmath>
#include "Animation.h"
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "ecs/components/Player.h"
#include "ecs/components/Animator.h"

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
	  
	  // calculate step
	  Vector2 step = CalculateStep(body.GetForce(), body.GetVelocity());
	  player.SetStateFromStep(step);

	  // change sprite animation
	  adv::Sprite& sprite = registry.GetComponent<adv::Sprite>(e);
	  adv::Animator& anim = registry.GetComponent<adv::Animator>(e);
	  anim.ChangeTo(player.GetState());
	  sprite.SetIndex((size_t)anim.Value());
	  
	  // apply step
	  step.x *= player.IsJumping() ? H_JUMP_MULT : H_NO_JUMP_MULT;
	  step.y *= V_MULT;
	  body.ApplyAcc(step);
	}
  }

}

Vector2 PlayerSystem::CalculateStep(Vector2 force, Vector2 velocity)
{
  Vector2 step = Vector2Zero();
	
  step.x = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
	
  bool floating = adv::IsWorldUp(force) >= V_MV_TOLERANCE;
  bool movingVert = std::abs(adv::IsWorldUp(velocity)) >= V_MV_TOLERANCE;
  if (!(movingVert || floating)) 
	step.y = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));

  return step;
}
