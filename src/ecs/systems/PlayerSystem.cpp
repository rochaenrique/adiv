#include "PlayerSystem.h"
#include <iostream>
#include <cmath>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "ecs/components/Animator.h"
#include "util/Helper.h"

extern Registry registry;

void PlayerSystem::Update(float)
{ 
  for (const Entity& e : m_Entities) {
	adv::RigidBody& body = registry.GetComponent<adv::RigidBody>(e);
	adv::Player& player = registry.GetComponent<adv::Player>(e);
	  
	// calculate step
	Vector2 step = CalculateStep(body.GetForce(), body.GetVelocity(), player.IsJumping());
	player.SetStateFromStep(step);

	// change sprite animation
	adv::Sprite& sprite = registry.GetComponent<adv::Sprite>(e);
	adv::Animator& anim = registry.GetComponent<adv::Animator>(e);
	anim.ChangeTo(player.GetState());
	sprite.SetIndex(static_cast<size_t>(anim.Value()));
	  
	// apply step
	step.x *= H_MULT;
	step.y *= V_MULT;
	body.ApplyAcc(step);
  }

}

Vector2 PlayerSystem::CalculateStep(Vector2 force, Vector2 velocity, bool jumping)
{
  Vector2 step = Vector2Zero();

  
  bool floating = std::abs(adv::IsWorldUp(force)) >= V_MV_TOLERANCE;
  bool movingVert = std::abs(adv::IsWorldUp(velocity)) >= V_MV_TOLERANCE;

  if (!(floating || jumping || movingVert)) 
	step = {
	  static_cast<float>(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
	  static_cast<float>(IsKeyDown(KEY_S) - IsKeyDown(KEY_W))
	};

  return step;
}
