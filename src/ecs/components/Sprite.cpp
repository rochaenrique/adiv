#include "Sprite.h"
 
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

  void Sprite::IncrementSlotIndex(int)
  {
	// size_t total = texture->width / source.width;
	// i += GetSlotIndex();
	// if (i >= total) i %= total;
	// SetSlotIndex(i);
  };

  size_t Sprite::GetSlotIndex()
  {
	size_t x = source.x / texture->width * source.width;
	size_t y = source.x / texture->height * source.height;
	return x + y * (int)(texture->width / source.width);
  }
}

