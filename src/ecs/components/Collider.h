#pragma once
#include <raylib.h>
#include <raymath.h>

namespace adv
{
  enum ColliderType { CIRCLE, SQUARE };
  
  struct Collider { ColliderType type = SQUARE; };
  
  struct CircleCollider : public Collider
  {
	Vector2 center;
	float radius;
  };

  struct SquareCollider : public Collider
  {
	Rectangle rectangle;
  };

  struct CollisionPoints
  {
	CollisionPoints()
	  : a{0, 0}, b{0, 0}, normal{0,0}, collided{false} {}
	CollisionPoints(Vector2 a, Vector2 b, Vector2 n, bool c)
	  : a{a}, b{b}, normal{n}, collided{c} {}
	
	Vector2 a, b, normal;
	bool collided;
  };

  void UpdateCollider(Collider&, Vector2);

  CollisionPoints TestCircleCircle(const Collider&, const Collider&);
  CollisionPoints TestCircleSquare(const Collider&, const Collider&);
  CollisionPoints TestSquareSquare(const Collider&, const Collider&);

  CollisionPoints TestCollision(const Collider&, const Collider&);
}
