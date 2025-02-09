#pragma once
#include <raylib.h>
#include <raymath.h>
#include <memory>

namespace adv
{
  struct Sprite
  {
  Sprite()
	: texture{nullptr}, source{0, 0, 0, 0}
	{};
  Sprite(const std::shared_ptr<Texture2D>& t, Rectangle s)
	: texture{t}, source{s}
	{};
	
	std::shared_ptr<Texture2D> texture;
	Rectangle source;
	void IncrementSlotIndex(int);
	size_t GetSlotIndex();
	void SetSlotIndex(size_t);
  };
}
