#include "PhysicsSystem.h"
#include <iostream>
#include <raymath.h>
#include "ecs/Registry.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::RigidBody& rb = registry.GetComponent<adv::RigidBody>(e);
	if (rb.dynamic)
	  StepBody(
			   rb,
			   registry.GetComponent<adv::Transform>(e),
			   dt
			   );
  }
}

void PhysicsSystem::StepBody(adv::RigidBody& r, adv::Transform& t, float dt)
{
  r.AddForce(G);
  r.ApplyForces(dt);
  t.Displace(r.velocity * dt);
  r.ResetForce();
}
