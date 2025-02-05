#pragma once
#include <memory>
#include <raylib.h>

namespace adv
{
  struct Player    {};
  struct Gravity   { Vector2 force; };
  struct RigidBody { Vector2 acceleration, velocity; };
  struct Transform { Vector2 translation, scale, rotation; };
  struct Sprite
  {
	std::shared_ptr<Texture2D> texture;
	Rectangle source;
	void IncrementSlotIndex(int);
	size_t GetSlotIndex();
	void SetSlotIndex(size_t);
  };
}
