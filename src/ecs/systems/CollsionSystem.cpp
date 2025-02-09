#include "CollisionSystem.h"
#include "ecs/Registry.h"
#include "ecs/components/RigidBody.h"
#include "ecs/components/Transform.h"

#define PERCENT .4f
#define SLOP    .01f

extern Registry registry;

void CollisionSystem::Update(float)
{
  std::vector<adv::Collision> collisions;
  
  for (const Entity& a : m_Entities) 
	for (const Entity& b : m_Entities) {
	  if (a == b) break;
	  adv::Collider& A = registry.GetComponent<adv::Collider>(a);
	  adv::Transform& at = registry.GetComponent<adv::Transform>(a);
	  adv::Collider& B = registry.GetComponent<adv::Collider>(b);
	  adv::Transform& bt = registry.GetComponent<adv::Transform>(b);
	  A.UpdateCenter(at.translation);
	  B.UpdateCenter(bt.translation);

	  adv::CollisionPoints points = adv::Collider::TestCollision(A, B);
	  
	  if (points.collided) 
		collisions.emplace_back(a, b, points);
	}

  ImpulseSolver(collisions);
  PositionSolver(collisions);
}

void CollisionSystem::ImpulseSolver(std::vector<adv::Collision>& cols)
{
  for (const adv::Collision& col : cols) {
	adv::RigidBody& ar = registry.GetComponent<adv::RigidBody>(col.a);
	adv::RigidBody& br = registry.GetComponent<adv::RigidBody>(col.b);
	Vector2 avel = ar.velocity;
	Vector2 bvel = br.velocity;
	Vector2 normal = col.points.normal;
	  
	float aInvmass = ar.InvMass();
	float bInvmass = br.InvMass();
	  
	Vector2 rvel = avel - bvel;
	float speed = Vector2DotProduct(rvel, normal);
	if (speed >= 0) continue;

	float restitution = (ar.restitution * br.restitution);
	float j = -(1.0f + restitution) * speed / (aInvmass + bInvmass);
	Vector2 impulse = normal * j;
	if (ar.dynamic) avel += impulse * aInvmass;
	if (br.dynamic) bvel -= impulse * bInvmass;

	// friction
	rvel = avel - bvel;
	speed = Vector2DotProduct(rvel, normal);
	Vector2 tangent = rvel - normal * speed;
	if (Vector2Length(tangent) > 0.0001f) tangent = Vector2Normalize(tangent);

	float asf = ar.dynamic ? ar.staticFriction  : 0.0f;
	float adf = ar.dynamic ? ar.dynamicFriction : 0.0f;
	float bsf = br.dynamic ? br.staticFriction  : 0.0f;
	float bdf = br.dynamic ? br.dynamicFriction : 0.0f;

	float fvel = Vector2DotProduct(rvel, tangent);
	float mu = Vector2Length({ asf, bsf });
	float f = -fvel / (aInvmass + bInvmass);
	  
	Vector2 friction;
	if (std::abs(f) < j * mu)
	  friction = tangent * f;
	else {
	  mu = Vector2Length({ adf, bdf });
	  friction = tangent * -j * mu;
	}

	if (ar.dynamic) ar.velocity = avel + friction * aInvmass;
	if (br.dynamic) br.velocity = bvel - friction * bInvmass;
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

	if (ar.dynamic) at.translation += correction * aInvmass;
	if (br.dynamic) bt.translation -= correction * bInvmass;
  }
}

