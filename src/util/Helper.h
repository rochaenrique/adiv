#pragma once
#include <raylib.h>
#include "ecs/Components.h"

namespace adv
{
  Rectangle ToRect(const adv::Transform& t);
  
  Rectangle ReCenter(const Rectangle& r);
  Rectangle GetCenteredRect(const adv::Transform& t);
  
  void ToTop(adv::Transform&, float);
  void ToBottom(adv::Transform&, float);
  void ToLeft(adv::Transform&, float);
  void ToRight(adv::Transform&, float);
}

