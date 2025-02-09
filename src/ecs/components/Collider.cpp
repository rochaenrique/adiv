#include "Collider.h"
#include <cmath>
#include <cassert>
#include <raylib.h>
#include <iostream>
#include "ecs/Components.h"
#include "ecs/Registry.h"
#include "util/Helper.h"

extern Registry registry;

namespace adv
{
  CollisionPoints Collider::TestCollision(Collider& a, Collider& b)
  {
	if (CheckCollisionRecs(adv::ReCenter(a.rectangle), adv::ReCenter(b.rectangle))) {
	  Vector2 ac = { a.rectangle.x, a.rectangle.y };
	  Vector2 bc = { b.rectangle.x, b.rectangle.y };
	  
	  Vector2 normal = Vector2Normalize(ac - bc);
	  
	  return CollisionPoints(Vector2Zero(), Vector2Zero(), normal, true);
	}
	
	return CollisionPoints();
  };
  
}
