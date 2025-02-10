#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <functional>
#include "ecs/ECS.h"

namespace adv
{
  struct CollisionPoints
  {
  CollisionPoints()
	: a{0, 0}, b{0, 0}, normal{0,0}, collided{false} {};
  CollisionPoints(Vector2 a, Vector2 b, Vector2 n, bool c)
	: a{a}, b{b}, normal{n}, collided{c} {};
	
	Vector2 a, b, normal;
	bool collided;
  };

  struct Collider
  {
	Collider()
	  : rectangle{0, 0, 0, 0} {};
	
	Collider(Rectangle r)
	  : rectangle{r} {};

	Collider(float w, float h)
	  : rectangle{0, 0, w, h} {};

	Collider(Vector2 sz)
	  : rectangle{0, 0, sz.x, sz.y} {};

	void UpdateCenter(const Vector2 c)
	{
	  rectangle.x = c.x;
	  rectangle.y = c.y;
	};
	
	using CollisionCallback = std::function<void(float)>;
	void SetCollisionCallback(const CollisionCallback& func)
	{
	  onCollision = func;
	  trigger = true;
	};
	bool IsTrigger() const { return trigger; };
	static CollisionPoints TestCollision(Collider&, Collider&);
	
	Rectangle rectangle;
	CollisionCallback onCollision;
  private:
	bool trigger = false;
  };

  struct Collision
  {
	Entity a, b;
	CollisionPoints points;
  };
}
