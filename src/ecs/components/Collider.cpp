#include "Collider.h"
#include <raylib.h>
#include <cassert>
#include <iostream>

namespace adv
{ 
  void UpdateCollider(Collider& col, const Vector2 c)
  {
	switch (col.type) {
	case CIRCLE:
	  ((CircleCollider&)col).center = c;
	  std::cout << "CIRCLE HERE\n";
	  break;
	case SQUARE:
	  auto& s = ((SquareCollider&)col);
	  s.rectangle.x = c.x - s.rectangle.width;
	  s.rectangle.y = c.y - s.rectangle.height;
	  std::cout << "SQUARE HERE\n";
	  break;
	}
  };

  CollisionPoints TestCircleCircle(const Collider& a, const Collider& b)
  {
	assert(a.type == ColliderType::CIRCLE && b.type == ColliderType::CIRCLE);
	CircleCollider& A = (CircleCollider&)a;
	CircleCollider& B = (CircleCollider&)b;

	if (CheckCollisionCircles(A.center, A.radius, B.center, B.radius)) {
	  Vector2 normal = Vector2Normalize(A.center - B.center);
	  Vector2 afar = A.center + normal * A.radius;
	  Vector2 bfar = B.center + normal * B.radius;
	  return CollisionPoints(afar, bfar, normal, true);
	}
	return CollisionPoints();
  }
  
  CollisionPoints TestCircleSquare(const Collider& a, const Collider& b)
  {
	assert(a.type == ColliderType::CIRCLE && b.type == ColliderType::SQUARE);

	CircleCollider& A = (CircleCollider&)a;
	SquareCollider& B = (SquareCollider&)b;

	if (CheckCollisionCircleRec(A.center, A.radius, B.rectangle)) {
	  Vector2 Bcenter = { B.rectangle.x + B.rectangle.width, B.rectangle.y + B.rectangle.height };
	  Vector2 normal = Vector2Normalize(A.center - Bcenter);
	  Vector2 afar = A.center + normal * A.radius;

	  return CollisionPoints(afar, Vector2Zero(), normal, true);
	}
	return CollisionPoints();
  }
  
  CollisionPoints TestSquareSquare(const Collider& a, const Collider& b)
  {
	assert(a.type == ColliderType::SQUARE && b.type == ColliderType::SQUARE);

	SquareCollider& A = (SquareCollider&)a;
	SquareCollider& B = (SquareCollider&)b;

	if (CheckCollisionRecs(A.rectangle, B.rectangle)) {
	  Vector2 Acenter = { A.rectangle.x + A.rectangle.width, A.rectangle.y + A.rectangle.height };
	  Vector2 Bcenter = { B.rectangle.x + B.rectangle.width, B.rectangle.y + B.rectangle.height };
	  Vector2 normal = Vector2Normalize(Acenter - Bcenter);
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  }

  using ContactFunc = CollisionPoints(*)(const Collider&, const Collider&);

  CollisionPoints TestCollision(const Collider& a, const Collider& b)
  {
	static const ContactFunc tests[2][2] = {
	  { TestCircleCircle, TestCircleSquare },
	  { nullptr,          TestSquareSquare }, 
	};
	
	bool swap = a.type > b.type;
	std::cout << "atype: " << a.type << ", btype: " << b.type << std::endl;
	CollisionPoints points = swap ? tests[b.type][a.type](b, a) : tests[a.type][b.type](a, b);

	if (swap) {
	  std::swap(points.a, points.b);
	  points.normal = Vector2Negate(points.normal);
	}

	return points;
  };
  
}
