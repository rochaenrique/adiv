#pragma once
#include <raymath.h>
#include <iostream>

namespace adv
{
  class RigidBody
  {
  public:
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

	void ApplyForce(Vector2 f)
	{
	  force += f;
	};
	void ApplyAcc(Vector2 a)
	{
	  force += a * mass;
	};
	void ResolveForces(float dt)
	{
	  velocity += force * InvMass() * dt;
	}
	void ResetForce()
	{
	  lastForce = force;
	  force = Vector2Zero();
	};

	void ApplyVelocity(Vector2 v)
	{
	  velocity += v;
	};
	
	Vector2 GetForce() const { return force; }	
	Vector2 GetVelocity() const { return velocity; }
	Vector2 GetLastForce() const { return lastForce; }

	float GetMass() const { return mass; }
	float InvMass() const { return (mass != 0.0f) ? (1.0f / mass) : 0.0f; }

	float GetStaticFriction() const { return staticFriction; }
	float GetDynamicFriction() const { return dynamicFriction; }
	float GetRestitution() const { return restitution; }

	bool IsDynamic() { return dynamic; };
  private:	
	Vector2 force, velocity, lastForce = Vector2Zero();
	float mass, staticFriction, dynamicFriction, restitution;
	bool dynamic;
  };
}
