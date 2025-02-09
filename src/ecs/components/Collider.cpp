#include "Collider.h"
#include "Components.h"
#include "ecs/Registry.h"
#include <raylib.h>
#include <cassert>
#include <cmath>
#include <iostream>

extern Registry registry;

namespace adv
{
  CollisionPoints Collider::TestCollision(Collider& a, Collider& b)
  {
	if (CheckCollisionRecs(a.rectangle, b.rectangle)) {
	  Vector2 ac = { a.rectangle.x + a.rectangle.width, a.rectangle.y + b.rectangle.height };
	  Vector2 bc = { b.rectangle.x + b.rectangle.width, b.rectangle.y + b.rectangle.height };
	  Vector2 normal = Vector2Normalize(ac - bc);
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  };
  
  void ImpulseSolver(std::vector<Collision>& cols)
  {

	for (const Collision& col : cols) {
	  // TODO: distinguish dynamic and non dynamic objects
	  adv::RigidBody& ar = registry.GetComponent<adv::RigidBody>(col.a);
	  adv::RigidBody& br = registry.GetComponent<adv::RigidBody>(col.b);
	  Vector2 avel = ar.velocity;
	  Vector2 bvel = br.velocity;
	  Vector2 normal = col.points.normal;
	  
	  assert(ar.mass != 0 && br.mass != 0 && "Mass equal to 0!");
	  float aInvmass = 1.0f / ar.mass;
	  float bInvmass = 1.0f / br.mass;
	  
	  Vector2 rvel = avel - bvel;
	  float speed = Vector2DotProduct(rvel, normal);
	  if (speed >= 0) continue;
	  
	  static const float restitution = 1.0f; // TOOD: add restitution
	  float j = -(1.0f + restitution) * speed / (aInvmass + bInvmass);

	  Vector2 impulse = normal * j;
	  avel += impulse * aInvmass;
	  bvel -= impulse * bInvmass;

	  // friction
	  rvel = avel - bvel;
	  speed = Vector2DotProduct(rvel, normal);
	  Vector2 tangent = rvel - normal * speed;
	  if (Vector2Length(tangent) > 0.0001f) tangent = Vector2Normalize(tangent);

	  float fvel = Vector2DotProduct(rvel, tangent);
	  float mu = Vector2Length({ ar.staticFriction, br.staticFriction });
	  float f = -fvel / (aInvmass + bInvmass);
	  
	  Vector2 friction;
	  if (std::abs(f) < j * mu)
		friction = tangent * f;
	  else {
		mu = Vector2Length({ ar.dynamicFriction, br.dynamicFriction });
		friction = tangent * -j * mu;
	  }

	  ar.velocity = avel + friction * aInvmass;
	  br.velocity = bvel - friction * bInvmass;
	}
  };

  void PositionSolver(std::vector<Collision>& cols)
  {
	for (const Collision& col : cols) {
	  adv::RigidBody& ar = registry.GetComponent<adv::RigidBody>(col.a);
	  adv::RigidBody& br = registry.GetComponent<adv::RigidBody>(col.b);
	  
	  assert(ar.mass != 0 && br.mass != 0 && "Mass equal to 0!");
	  float aInvmass = 1.0f / ar.mass;
	  float bInvmass = 1.0f / br.mass;
	  Vector2 normal = col.points.normal;

	  static const float percent = .4f;
	  static const float slop = .01f;

	  Vector2 correction = normal * percent
		* std::fmax(Vector2Length(normal) - slop, 0.0f)
		/ (aInvmass + bInvmass);

	  adv::Transform& at = registry.GetComponent<adv::Transform>(col.a);
	  adv::Transform& bt = registry.GetComponent<adv::Transform>(col.b);

	  at.translation += correction * aInvmass;
	  bt.translation -= correction * bInvmass;
	}
  }

}
