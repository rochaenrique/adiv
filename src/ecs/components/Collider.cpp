#include "Collider.h"
#include <raylib.h>
#include <cassert>
#include <iostream>

namespace adv
{
  void UpdateCollider(CircleCollider& col, const Vector2 c)
  {
	col.center = c;
  }

  void UpdateCollider(SquareCollider& col, const Vector2 c)
  {
	col.rectangle.x = c.x - col.rectangle.width;
	col.rectangle.y = c.y - col.rectangle.height;
  };

  CollisionPoints TestCircleCircle(Collider& a, const Vector2 at, Collider& b, const Vector2 bt)
  {
	assert(a.type == ColliderType::CIRCLE && b.type == ColliderType::CIRCLE);
	CircleCollider A = static_cast<CircleCollider>(a);
	CircleCollider B = static_cast<CircleCollider>(b);
	UpdateCollider(A, at);
	UpdateCollider(B, bt);

	if (CheckCollisionCircles(A.center, A.radius, B.center, B.radius)) {
	  Vector2 normal = Vector2Normalize(A.center - B.center);
	  Vector2 afar = A.center + normal * A.radius;
	  Vector2 bfar = B.center + normal * B.radius;
	  return CollisionPoints(afar, bfar, normal, true);
	}
	return CollisionPoints();
  }
  
  CollisionPoints TestCircleSquare(Collider& a, const Vector2 at, Collider& b, const Vector2 bt)
  {
	assert(a.type == ColliderType::CIRCLE && b.type == ColliderType::SQUARE);

	CircleCollider A = static_cast<CircleCollider>(a);
	SquareCollider B = static_cast<SquareCollider>(b);
	UpdateCollider(A, at);
	UpdateCollider(B, bt);

	if (CheckCollisionCircleRec(A.center, A.radius, B.rectangle)) {
	  Vector2 Bcenter = { B.rectangle.x + B.rectangle.width, B.rectangle.y + B.rectangle.height };
	  Vector2 normal = Vector2Normalize(A.center - Bcenter);
	  Vector2 afar = A.center + normal * A.radius;

	  return CollisionPoints(afar, Vector2Zero(), normal, true);
	}
	return CollisionPoints();
  }
  
  CollisionPoints TestSquareSquare(Collider& a, const Vector2 at, Collider& b, const Vector2 bt)
  {
	assert(a.type == ColliderType::SQUARE && b.type == ColliderType::SQUARE);

	SquareCollider A = static_cast<SquareCollider>(a);
	SquareCollider B = static_cast<SquareCollider>(b);
	UpdateCollider(A, at);
	UpdateCollider(B, bt);

	if (CheckCollisionRecs(A.rectangle, B.rectangle)) {
	  Vector2 Acenter = { A.rectangle.x + A.rectangle.width, A.rectangle.y + A.rectangle.height };
	  Vector2 Bcenter = { B.rectangle.x + B.rectangle.width, B.rectangle.y + B.rectangle.height };
	  Vector2 normal = Vector2Normalize(Acenter - Bcenter);
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  }

  using ContactFunc = CollisionPoints(*)(Collider&, const Vector2, Collider&, const Vector2);

  CollisionPoints TestCollision(Collider& a, const Vector2 at, Collider& b, const Vector2 bt)
  {
	static const ContactFunc tests[2][2] = {
	  { TestCircleCircle, TestCircleSquare },
	  { nullptr,          TestSquareSquare }, 
	};
	
	bool swap = a.type > b.type;
	assert(a.type >= 0 && a.type <= 2 && "Collider type is not valid");
	assert(b.type >= 0 && b.type <= 2 && "Collider type is not valid");

	CollisionPoints points = swap ? tests[b.type][a.type](b, bt, a, at) : tests[a.type][b.type](a, at, b, bt);

	if (swap) {
	  std::swap(points.a, points.b);
	  points.normal = Vector2Negate(points.normal);
	}

	return points;
  };
  
}
