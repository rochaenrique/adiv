#pragma once
#include <raylib.h>

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
	
	Vector2 force, velocity;
	float mass, staticFriction, dynamicFriction, restitution;
	bool dynamic;

	float InvMass() const
	{
	  return (mass != 0.0f) ? 1.0f / mass : 0.0f;
	};
  };
}
