#pragma once
#include <raylib.h>
#include "ecs/Components.h"

namespace adv
{
  Rectangle ToRect(const adv::Transform& t);
  
  Rectangle ReCenter(const Rectangle& r);
  Rectangle GetCenteredRect(const adv::Transform& t);

  void ToBottom(adv::Transform& t, float y);
}

