#pragma once
#include <raylib.h>

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

  Rectangle ReCenter(const adv::Transform& t)
  {
	return {
	  t.translation.x - (t.scale.x / 2.0f),
	  t.translation.y - (t.scale.y / 2.0f),
	  t.scale.x,
	  t.scale.y
	};
  }

}
