#include "Helper.h"

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
  
  void ToBottom(adv::Transform& t, float y)
  {
	t.translation.y = y - t.scale.y / 2.0f;
  }
}
