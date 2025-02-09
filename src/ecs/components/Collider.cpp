#include "Collider.h"
#include <cmath>
#include <cassert>
#include <raylib.h>
#include <iostream>
#include "Components.h"
#include "ecs/Registry.h"
#include "util/Helper.h"

#define PERCENT .4f
#define SLOP    .01f

extern Registry registry;

namespace adv
{
  CollisionPoints Collider::TestCollision(Collider& a, Collider& b)
  {
	if (CheckCollisionRecs(adv::ReCenter(a.rectangle), adv::ReCenter(b.rectangle))) {
	  Vector2 ac = { a.rectangle.x, a.rectangle.y };
	  Vector2 bc = { b.rectangle.x, b.rectangle.y };
	  
	  Vector2 normal = Vector2Normalize(ac - bc);
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  };
  
  void ImpulseSolver(std::vector<Collision>& cols)
  {

	for (const Collision& col : cols) {
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
  };

  void PositionSolver(std::vector<Collision>& cols)
  {
	for (const Collision& col : cols) {
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
}
