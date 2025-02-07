#pragma once
#include <raylib.h>
#include <raymath.h>

namespace adv
{
  struct Collider { Rectangle rectangle; };
  
  struct CollisionPoints
  {
	CollisionPoints()
	  : a{0, 0}, b{0, 0}, normal{0,0}, collided{false} {}
	CollisionPoints(Vector2 a, Vector2 b, Vector2 n, bool c)
	  : a{a}, b{b}, normal{n}, collided{c} {}
	
	Vector2 a, b, normal;
	bool collided;
  };

  void UpdateCollider(Collider&, const Vector2);

  CollisionPoints TestCollision(Collider&, const Vector2, Collider&, const Vector2);
}
