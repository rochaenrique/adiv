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

  struct Collision
  {
	Entity a, b;
	CollisionPoints points;
  };

  class Collider
  {
  public:
	Collider()
	  : rectangle{0, 0, 0, 0}, m_IsMap{false} {};
	
	Collider(float w, float h, bool m)
	  : rectangle{0, 0, w, h}, m_IsMap{m} {};

	Collider(Vector2 sz, bool m)
	  : rectangle{0, 0, sz.x, sz.y}, m_IsMap{m} {};

	void UpdateCenter(const Vector2 c)
	{
	  rectangle.x = c.x;
	  rectangle.y = c.y;
	};
	
	using CollisionCallback = std::function<void(const adv::Collision&, float)>;
	void SetCollisionCallback(const CollisionCallback& func)
	{
	  onCollision = func;
	  m_Trigger = true;
	};
	bool IsTrigger() const { return m_Trigger; };
	bool IsMap() const { return m_IsMap; };
	static CollisionPoints TestCollision(Collider&, Collider&);
	
	Rectangle rectangle;
	CollisionCallback onCollision;
  private:
	bool m_IsMap;
	bool m_Trigger = false;
  };

}
