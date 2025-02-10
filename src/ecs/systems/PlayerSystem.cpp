#include "PlayerSystem.h"
#include <iostream>
#include <raylib.h>
#include <raylib.h>
#include <cmath>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"

#define VERT_MULT 100000.0f
#define HOR_MULT  50.0f

extern Registry registry;

void PlayerSystem::Update(float)
{
  std::cout << "Players Count: " << m_Entities.size() << '\n';
  for (const Entity& e : m_Entities) {
	adv::RigidBody& body = registry.GetComponent<adv::RigidBody>(e);
	bool canJump = std::abs(adv::IsWorldUp(body.velocity)) <= 1.0f;
	if (IsKeyPressed(KEY_R)) {
	  adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	  t.translation = Vector2Zero();
	  t.rotation    = Vector2Zero();
	} else if (canJump) { 
	  Vector2 step = {
		(float)(IsKeyPressed(KEY_D) - IsKeyPressed(KEY_A)) * VERT_MULT,
		(float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * HOR_MULT,
	  };
	  body.AddForce(step);
	}
  }
}
