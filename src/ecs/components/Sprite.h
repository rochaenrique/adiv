#pragma once
#include <raylib.h>
#include <raymath.h>
#include <memory>

namespace adv
{
  class Sprite
  {
  public:
  Sprite()
	: texture{nullptr}, source{0, 0, 0, 0}
	{};
  Sprite(const std::shared_ptr<Texture2D>& t, Rectangle s)
	: texture{t}, source{s}
	{};
	
	void IncrementSlotIndex(int);
	size_t GetSlotIndex();
	void SetSlotIndex(size_t);

	const std::shared_ptr<Texture2D> GetTexture() const { return texture; }
	const Rectangle& GetSource() const { return source; }
  private:
	std::shared_ptr<Texture2D> texture;
	Rectangle source;
  };
}
