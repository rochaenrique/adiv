#pragma once
#include <raylib.h>
#include "ecs/Components.h"

#define WORLD_UP	Vector2{  0.0f, -1.0f }
#define WORLD_DOWN  Vector2{  0.0f,  1.0f }
#define WORLD_LEFT  Vector2{ -1.0f,  0.0f }
#define WORLD_RIGHT Vector2{  1.0f,  0.0f }

namespace adv
{
  Rectangle ToRect(const adv::Transform& t);
  
  Rectangle ReCenter(const Rectangle& r);
  Rectangle GetCenteredRect(const adv::Transform& t);
  
  void ToTop(adv::Transform&, float);
  void ToBottom(adv::Transform&, float);
  void ToLeft(adv::Transform&, float);
  void ToRight(adv::Transform&, float);

  float IsWorldUp(Vector2); 
  float IsWorldDown(Vector2);
  float IsWorldLeft(Vector2);
  float IsWorldRight(Vector2);
}

std::ostream& operator<<(std::ostream&, const Vector2&);
std::ostream& operator<<(std::ostream&, const adv::RigidBody&);
std::ostream& operator<<(std::ostream&, const adv::Player&);

