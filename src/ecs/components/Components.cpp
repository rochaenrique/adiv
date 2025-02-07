#include "Components.h"

namespace adv
{
  
  void Sprite::SetSlotIndex(size_t i)
  {
	size_t total = texture->width / source.width;
	if (i < total) { 
	  source.x = source.width * (total % i);
	  source.y = source.height * (total / i);
	}
  }

  void Sprite::IncrementSlotIndex(int i)
  {
	size_t total = texture->width / source.width;
	i += GetSlotIndex();
	if (i >= total) i %= total;
	SetSlotIndex(i);
  };

  size_t Sprite::GetSlotIndex()
  {
	size_t x = source.x / texture->width * source.width;
	size_t y = source.x / texture->height * source.height;
	return x + y * (int)(texture->width / source.width);
  }

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
