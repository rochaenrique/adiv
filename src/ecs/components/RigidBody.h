#pragma once
#include <raymath.h>
#include <iostream>

namespace adv
{
  struct RigidBody
  {
  RigidBody()
	: force{Vector2Zero()}, velocity{Vector2Zero()},
	  mass{1.0f}, staticFriction{0}, dynamicFriction{0}, restitution{1.0f},
	  dynamic{true}
	{};
	
  RigidBody(Vector2 f, Vector2 v, float m, float sf, float df, float r, bool d)
	: force{f}, velocity{v},
	  mass{m}, staticFriction{sf}, dynamicFriction{df}, restitution{r},
	  dynamic{d}
	{};

	static RigidBody CreateStatic(float sf, float df)
	{
	  return RigidBody(Vector2Zero(), Vector2Zero(), 0.0f, sf, df, 1.0f, false);
	};

	void AddForce(Vector2 f) { force += f * mass; };
	void ApplyForces(float dt) { velocity += force * InvMass() * dt; }
	void ResetForce()
	{
	  lastForce = force;
	  force = Vector2Zero();
	};
	float InvMass() const
	{
	  return (mass != 0.0f) ? 1.0f / mass : 0.0f;
	};
	
	Vector2 force, velocity, lastForce = Vector2Zero();
	float mass, staticFriction, dynamicFriction, restitution;
	bool dynamic;
  };
}
