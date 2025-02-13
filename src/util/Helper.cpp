#include "Helper.h"
#include <iomanip>
#include <string>
#include <iterator>

#define OVERLAP_TH .0001f

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

  Vector2 CalculateMapNormal(const Rectangle& a, const Rectangle& b)
  {
	float xoverlap = std::min(a.x + a.width, b.x + b.width) - std::max(a.x, b.x);
	float yoverlap = std::min(a.y + a.height, b.y + b.height) - std::max(a.y, b.y);

	if (std::fabs(xoverlap) < OVERLAP_TH) xoverlap = 0;
	if (std::fabs(yoverlap) < OVERLAP_TH) yoverlap = 0;

	return (xoverlap < yoverlap)
	  ? Vector2{ a.x > b.x ? 1.0f : -1.0f, 0.0f }
	  : Vector2{ 0.0f, a.y > b.y ? 1.0f : -1.0f };
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
	
  float IsWorldDown(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_DOWN);
  }
	
  float IsWorldLeft(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_LEFT);
  }

  float IsWorldRight(Vector2 v)
  {
	return Vector2DotProduct(v, WORLD_RIGHT);
  }

  Vector2 ToVector2(const std::string& s)
  {
	Vector2 v = Vector2Zero();
	auto it = s.begin();
	while (it != s.end() && std::isdigit(*it)) {
	  v.x = v.x * 10 + (*it - '0');
	  it++; ;
	}
	it++; 
	while (it != s.end() && std::isdigit(*it)) {
	  v.y = v.y * 10 + (*it - '0');
	  it++; 
	}
	
	return v;
  }
  
  Vector2 ToVector2(std::string::iterator& it, const std::string::iterator end)
  {
	Vector2 v = Vector2Zero();
	while (it != end && std::isdigit(*it)) {
	  v.x = v.x * 10 + (*it - '0');
	  it++;
	}
	if (it == end) return v;
	it++;
	while (it != end && std::isdigit(*it)) {
	  v.y = v.y * 10 + (*it - '0');
	  it++;
	}

	return v;
  };
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
  os << "(" << vec.x << ", " << vec.y << ")" << std::setprecision(3);
  return os;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect)
{
  os << "x: " << rect.x << ", y: " << rect.y << ", width: "
	 << rect.width << ", height: " << rect.height << std::setprecision(3);
  return os;
}

std::ostream& operator<<(std::ostream& os, const adv::RigidBody& rb)
{
  os << "F: " << rb.GetForce() << ", Last F: " << rb.GetLastForce() << ", Vel: " << rb.GetVelocity();
  return os;
}

std::ostream& operator<<(std::ostream& os, const adv::Transform& tr)
{
  os << "xy: " << tr.translation << ", scale: " << tr.scale << ", rotation: " << tr.rotation;
  return os;
}

std::ostream& operator<<(std::ostream& os, const adv::Player& p)
{
  os << "Jumping: " << p.IsJumping();
  return os;
}

