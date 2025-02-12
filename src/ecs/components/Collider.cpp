#include "Collider.h"
#include <cassert>
#include <raylib.h>
#include "ecs/Registry.h"
#include "util/Helper.h"

extern Registry registry;

namespace adv
{
  CollisionPoints Collider::TestCollision(Collider& a, Collider& b)
  {
	Rectangle acent = adv::ReCenter(a.rectangle);
	Rectangle bcent = adv::ReCenter(b.rectangle);

	if (CheckCollisionRecs(acent, bcent)) {
	  Vector2 normal = Vector2Normalize(Vector2{ a.rectangle.x, a.rectangle.y } - Vector2{ b.rectangle.x, b.rectangle.y });
	  if (a.IsMap() || b.IsMap()) 
		normal = adv::CalculateMapNormal(acent, bcent);

	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	return CollisionPoints();
  };

}
