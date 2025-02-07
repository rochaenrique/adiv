#include "Collider.h"
#include <raylib.h>
#include <cassert>
#include <iostream>

namespace adv
{
  void UpdateCollider(Collider& col, const Vector2 c)
  {
	col.rectangle.x = c.x;
	col.rectangle.y = c.y;
  };

  CollisionPoints TestCollision(Collider& a, const Vector2 at, Collider& b, const Vector2 bt)
  {
	UpdateCollider(a, at);
	UpdateCollider(b, bt);

	if (CheckCollisionRecs(a.rectangle, b.rectangle)) {
	  Vector2 ac = { a.rectangle.x + a.rectangle.width, a.rectangle.y + b.rectangle.height };
	  Vector2 bc = { b.rectangle.x + b.rectangle.width, b.rectangle.y + b.rectangle.height };
	  Vector2 normal = Vector2Normalize(ac - bc);
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  };
  
}
