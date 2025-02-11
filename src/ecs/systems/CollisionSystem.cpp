#include "CollisionSystem.h"
#include <iostream>
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"

#define PERCENT .4f
#define SLOP    .01f

extern Registry registry;

void CollisionSystem::Update(float dt)
{
  std::vector<adv::Collision> collisions;
  for (const Entity& a : m_Entities) 
	for (const Entity& b : m_Entities) {
	  if (a == b) break;
	  adv::Collider& A = registry.GetComponent<adv::Collider>(a);
	  adv::Collider& B = registry.GetComponent<adv::Collider>(b);
	  
	  adv::Transform& at = registry.GetComponent<adv::Transform>(a);
	  adv::Transform& bt = registry.GetComponent<adv::Transform>(b);
	  
	  A.UpdateCenter(at.translation);
	  B.UpdateCenter(bt.translation);

	  adv::CollisionPoints points = adv::Collider::TestCollision(A, B);
	  
	  if (points.collided) 
		collisions.emplace_back(a, b, points);
	}

  ImpulseSolver(collisions);
  PositionSolver(collisions);
  SendSignals(collisions, dt);
}

void CollisionSystem::ImpulseSolver(std::vector<adv::Collision>& cols)
{
  for (const adv::Collision& col : cols) {
	adv::RigidBody& ar = registry.GetComponent<adv::RigidBody>(col.a);
	adv::RigidBody& br = registry.GetComponent<adv::RigidBody>(col.b);
	Vector2 normal = col.points.normal;
	  
	float aInvmass = ar.InvMass();
	float bInvmass = br.InvMass();

	// impulse
	Vector2 rvel = ar.GetVelocity() - br.GetVelocity();
	float speed = Vector2DotProduct(rvel, normal);
	if (speed >= 0)
	  continue;

	float restitution = (ar.GetRestitution() * br.GetRestitution());
	float j = -(1.0f + restitution) * speed / (aInvmass + bInvmass);
	Vector2 impulse = normal * j;
	
	if (ar.IsDynamic()) ar.ApplyVelocity(impulse * aInvmass);
	if (br.IsDynamic()) br.ApplyVelocity(impulse * -bInvmass);

	// friction
	rvel = ar.GetVelocity() - br.GetVelocity();
	speed = Vector2DotProduct(rvel, normal);
	Vector2 tangent = Vector2Normalize(rvel - normal * speed);
	
	float mu = Vector2Length({ ar.GetStaticFriction(), br.GetStaticFriction() });
	float fvel = Vector2DotProduct(rvel, tangent);
	float f = -fvel / (aInvmass + bInvmass);
	
	Vector2 friction;
	if (std::abs(f) < j * mu)
	  friction = tangent * f;
	else {
	  mu = Vector2Length({ ar.GetDynamicFriction(), br.GetDynamicFriction() });
	  friction = tangent * -j * mu;
	}

	if (ar.IsDynamic()) ar.ApplyVelocity(friction * aInvmass);
	if (br.IsDynamic()) br.ApplyVelocity(friction * -bInvmass);
  }
}

void CollisionSystem::PositionSolver(std::vector<adv::Collision>& cols)
{
  for (const adv::Collision& col : cols) {
	adv::RigidBody& ar = registry.GetComponent<adv::RigidBody>(col.a);
	adv::RigidBody& br = registry.GetComponent<adv::RigidBody>(col.b);
	  
	float aInvmass = ar.InvMass();
	float bInvmass = br.InvMass();
	Vector2 normal = col.points.normal;

	Vector2 correction = normal * PERCENT
	  * std::fmax(Vector2Length(normal) - SLOP, 0.0f)
	  / (aInvmass + bInvmass);

	adv::Transform& at = registry.GetComponent<adv::Transform>(col.a);
	adv::Transform& bt = registry.GetComponent<adv::Transform>(col.b);

	if (ar.IsDynamic()) at.translation += correction * aInvmass;
	if (br.IsDynamic()) bt.translation -= correction * bInvmass;
  }
}

void CollisionSystem::SendSignals(std::vector<adv::Collision>& cols, float dt)
{
  for (const adv::Collision& col : cols) {
	adv::Collider& ac = registry.GetComponent<adv::Collider>(col.a);
	adv::Collider& bc = registry.GetComponent<adv::Collider>(col.b);

	if (ac.IsTrigger()) {
	  assert(ac.onCollision && "Collision marked as trigger has no callback!");
	  ac.onCollision(col, dt);
	}
	if (bc.IsTrigger()) {
	  assert(bc.onCollision && "Collision marked as trigger has no callback!");
	  bc.onCollision(col, dt);
	}
  }
}
