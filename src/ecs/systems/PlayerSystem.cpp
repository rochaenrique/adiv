#include "PlayerSystem.h"
#include <iostream>
#include <raylib.h>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"

#define VERT_MULT 1000.0f
#define HOR_MULT  50.0f

extern Registry registry;

void PlayerSystem::Update(float)
{
  std::cout << "Players Count: " << m_Entities.size() << '\n';
  for (const Entity& e : m_Entities) {
	//	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	if (IsKeyPressed(KEY_R)) {
	  adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	  t.translation = Vector2Zero();
	} else { 
	  Vector2 step = {
		(float)(IsKeyPressed(KEY_D) - IsKeyPressed(KEY_A)) * VERT_MULT,
		(float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * HOR_MULT,
	  };
	  adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);
	  r.AddForce(step);
	}
  }
}
