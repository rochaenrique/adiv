#include "PlayerSystem.h"
#include <raylib.h>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"

extern Registry registry;
void PlayerUpdateSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	//	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);
	//adv::Transform& t = registry.GetComponent<adv::Transform>(e);

	Vector2 step = {
	  (float)((int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A)),
	  (float)((int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W))
	};
	r.AddForce(step * 100.0f);
  }
}
