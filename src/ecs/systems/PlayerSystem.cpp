#include "PlayerSystem.h"
#include <iostream>
#include <raylib.h>
#include "util/Helper.h"
#include <cmath>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"

#define V_MV_TOLERANCE  2.0f
#define H_MV_TOLERANCE  1.0f
#define V_MULT          2500.0f
#define H_NO_JUMP_MULT  300.0f
#define H_JUMP_MULT     H_NO_JUMP_MULT * .1f

#define WALK_RIGHT_INDEX_BEGIN 0
#define WALK_RIGHT_INDEX_END   9
#define WALK_LEFT_INDEX_BEGIN  10
#define WALK_LEFT_INDEX_END    19
#define JUMP_INDEX       22
#define STANDING_INDEX   27

extern Registry registry;
extern size_t playerIndex;

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
	  adv::Sprite& sprite = registry.GetComponent<adv::Sprite>(e);
	  
	  // calculate step
	  Vector2 step = {};
	  Vector2 vel = body.GetVelocity();
	  if (std::abs(adv::IsWorldUp(vel)) <= V_MV_TOLERANCE && !player.IsJumping()) {
		step.y = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));
		if (step.y < 0) { 
		  player.SetJumping(true);
		  sprite.SetIndex(22);
		}
	  }
	  step.x = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));

	  // change sprite animation
	  if (player.IsJumping())
		sprite.SetIndex(JUMP_INDEX);
	  else if (std::abs(adv::IsWorldRight(vel)) <= H_MV_TOLERANCE) { 
		if (step.x < 0)
		  sprite.SetIndex(WALK_LEFT_INDEX_BEGIN);
		else if (step.x > 0)
		  sprite.SetIndex(WALK_RIGHT_INDEX_END);
		else
		  sprite.SetIndex(STANDING_INDEX);
	  } else {
		if (vel.x < 0) 
		  sprite.InRangeIncOrClamp(WALK_LEFT_INDEX_BEGIN, WALK_LEFT_INDEX_END);
	    else if (vel.x > 0)
		  sprite.InRangeIncOrClamp(WALK_RIGHT_INDEX_BEGIN, WALK_RIGHT_INDEX_END);
	  }
	  
	  // apply step
	  step.x *= player.IsJumping() ? H_JUMP_MULT : H_NO_JUMP_MULT;
	  step.y *= V_MULT;
	  body.ApplyAcc(step);

	  // TODO: REMOVE (TEST)

	  if (IsKeyPressed(KEY_Q)) {
		std::cout << "Attempting IncrementIndex\n";
		sprite.IncrementIndex();
	  }
	  if (IsKeyPressed(KEY_E)) {
		std::cout << "Attempting DecrementIndex\n";
		sprite.DecrementIndex();
	  }
	  playerIndex = sprite.GetIndex();
	}
  }
}
