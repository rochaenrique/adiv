#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include "Collider.h" // so you only need to include Components

namespace adv
{
  struct Player    {};
  struct RigidBody { Vector2 force, velocity; float mass; };
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
