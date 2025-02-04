#pragma once
#include "ECS.hpp"
#include "Registry.hpp"
#include "raylib.h"

extern Registry registry;

class PhysicsSystem : System
{
  void Update(float dt)
  {
	for (const Entity& e : m_Entities) {
	  Gravity& g   = registry.GetComponent<Gravity>(e);
	  auto& r = registry.GetComponent<RigidBody>(e);  
	  Transform& t = registry.GetComponent<Transform>(e);

	  t.translation.x += r.velocity.x * dt;
	  t.translation.y += r.velocity.y * dt;
	  
	  r.velocity.x += g.force.x * dt;
	  r.velocity.y += g.force.y * dt;
	}
  };
};
