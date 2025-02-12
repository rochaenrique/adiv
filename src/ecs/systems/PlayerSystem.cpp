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

void PlayerSystem::Update(float dt)
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
	  Vector2 step = {};
	  step.x = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
	  if (step.x < 0) player.SetState(adv::PlayerState::WALK_LEFT);
	  else if (step.x > 0) player.SetState(adv::PlayerState::WALK_RIGHT);
	  else player.SetState(adv::PlayerState::IDLE);

	  Vector2 vel = body.GetVelocity();
	  if (std::abs(adv::IsWorldUp(vel)) <= V_MV_TOLERANCE && !player.IsJumping()) {
		step.y = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));
		if (step.y < 0) { 
		  player.SetJumping(true);
		  player.SetState(adv::PlayerState::JUMP);
		}
	  }

	  // change sprite animation
	  adv::Sprite& sprite = registry.GetComponent<adv::Sprite>(e);
	  adv::Animator& anim = registry.GetComponent<adv::Animator>(e);
	  anim.Update(dt);
	  anim.ChangeTo(player.GetState());
	  sprite.SetIndex((size_t)anim.Value());
	  std::cout << "State: " << player.GetState() << ", Value: " <<
		(size_t)anim.Value() << '\n';
	  
	  // apply step
	  step.x *= player.IsJumping() ? H_JUMP_MULT : H_NO_JUMP_MULT;
	  step.y *= V_MULT;
	  body.ApplyAcc(step);
	}
  }
}
