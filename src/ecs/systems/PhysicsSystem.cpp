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
	if (!rb.IsDynamic())
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
  rb.ApplyAcc(G);
 rb.ResolveForces(dt);
  t.Displace(rb.GetVelocity() * dt);
  rb.ResetForce();
}
