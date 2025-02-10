#include "util/Helper.h"
#include "PhysicsSystem.h"
#include <iostream>
#include <raymath.h>
#include "ecs/Registry.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::RigidBody& rb = registry.GetComponent<adv::RigidBody>(e);
	if (!rb.dynamic)
	  continue;
	StepBody(
			 rb,
			 registry.GetComponent<adv::Transform>(e),
			 dt
			 );
  }
}

void PhysicsSystem::StepBody(adv::RigidBody& rb, adv::Transform& t, float dt)
{
  rb.ApplyForce(G);
  std::cout << "Body to update: " << rb << '\n';
  rb.ResolveForces(dt);
  t.Displace(rb.velocity * dt);
  rb.ResetForce();
}
