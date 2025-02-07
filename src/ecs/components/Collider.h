#pragma once
#include <raylib.h>
#include <raymath.h>

namespace adv
{
  enum ColliderType { CIRCLE, SQUARE };
  
  struct Collider
  {
	Collider() : type{ColliderType::SQUARE} {};
	Collider(ColliderType t) : type{t} {};
	ColliderType type;
  };
  
  struct CircleCollider : public Collider
  {
	CircleCollider(Vector2 c, float r)
	  : Collider(ColliderType::CIRCLE), center{c}, radius{r} {};
	CircleCollider(const Collider& c)
	  : Collider(ColliderType::CIRCLE)
	{
	  const CircleCollider& circ = static_cast<const CircleCollider&>(c);
	  center = circ.center;
	  radius = circ.radius;
	};
	Vector2 center;
	float radius;
  };

  struct SquareCollider : public Collider
  {
	SquareCollider(Rectangle r)
	  : Collider(ColliderType::SQUARE), rectangle{r} {};
	SquareCollider(const Collider& c)
	  : Collider(ColliderType::SQUARE)
	{
	  const SquareCollider& s = static_cast<const SquareCollider&>(c);
	  rectangle = s.rectangle;
	};
	
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

  void UpdateCollider(SquareCollider&, Vector2);
  void UpdateCollider(CircleCollider&, Vector2);

  CollisionPoints TestCircleCircle(Collider&, const Vector2, Collider&, const Vector2);
  CollisionPoints TestCircleSquare(Collider&, const Vector2, Collider&, const Vector2);
  CollisionPoints TestSquareSquare(Collider&, const Vector2, Collider&, const Vector2);
  CollisionPoints TestCollision(Collider&, const Vector2, Collider&, const Vector2);
}
