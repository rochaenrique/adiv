#include "PhysicsSystem.h"
#include <raymath.h>
#include "ecs/Registry.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) 
	StepBody(
			 registry.GetComponent<adv::RigidBody>(e),
			 registry.GetComponent<adv::Transform>(e),
			 dt
			 );
}

void PhysicsSystem::StepBody(adv::RigidBody& r, adv::Transform& t, float dt)
{
  r.AddForce(G);
  r.ApplyForces(dt);
  t.Displace(r.velocity * dt);
  r.ResetForce();
}
