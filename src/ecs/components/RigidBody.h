#pragma once
#include <raylib.h>

namespace adv
{
  struct RigidBody
  {
  RigidBody()
	: force{Vector2Zero()}, velocity{Vector2Zero()},
	  mass{1.0f}, staticFriction{0}, dynamicFriction{0},
	  dynamic{true}
	{};
  RigidBody(Vector2 f, Vector2 v, float m, float sf, float df, bool d)
	: force{f}, velocity{v},
	  mass{m}, staticFriction{sf}, dynamicFriction{df},
	  dynamic{d}
	{};
	
	Vector2 force, velocity;
	float mass, staticFriction, dynamicFriction;
	bool dynamic;
  };
  
}
