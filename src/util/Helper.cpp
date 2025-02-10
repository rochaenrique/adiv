#include "Helper.h"
#include <iomanip>

namespace adv
{
  Rectangle ReCenter(const Rectangle& r)
  {
	return {
	  r.x - (r.width / 2.0f),
	  r.y - (r.height / 2.0f),
	  r.width,
	  r.height
	};
  }

  Rectangle ToRect(const adv::Transform& t)
  {
	return {
	  t.translation.x, 
	  t.translation.y,
	  t.scale.x,
	  t.scale.y
	};
  }

  Rectangle GetCenteredRect(const adv::Transform& t)
  {
	return ReCenter(ToRect(t));
  }
  
  void ToTop(adv::Transform& t, float y)
  {
	t.translation.y = y + t.scale.y / 2.0f;
  }
  
  void ToBottom(adv::Transform& t, float y)
  {
	t.translation.y = y - t.scale.y / 2.0f;
  }
  
  void ToLeft(adv::Transform& t, float x)
  {
	t.translation.x = x + t.scale.x / 2.0f;
  }

  void ToRight(adv::Transform& t, float x)
  {
	t.translation.x = x - t.scale.x / 2.0f;
  }

  float IsWorldUp(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_UP);
  }
	
  bool IsWorldDown(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_DOWN) >= 0;
  }
	
  bool IsWorldLeft(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_LEFT) >= 0;
  }

  bool IsWorldRight(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_RIGHT) >= 0;
  }
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
  os << "(" << vec.x << ", " << vec.y << ")" << std::setprecision(3);
  return os;
}

std::ostream& operator<<(std::ostream& os, const adv::RigidBody& rb)
{
  os << "F: " << rb.GetForce() << ", Last F: " << rb.GetLastForce() << ", Vel: " << rb.GetVelocity();
  return os;
}

