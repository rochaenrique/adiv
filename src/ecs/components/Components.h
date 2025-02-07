#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

namespace adv
{
  struct Player    {};
  struct Transform
  {
  Transform(Vector2 t, Vector2 s, Vector2 r)
	: translation{t}, scale{s}, rotation{r}
	{};
  Transform()
	: translation{Vector2Zero()}, scale{Vector2Zero()}, rotation{Vector2Zero()}
	{};
	
	Vector2 translation, scale, rotation;
  };
  
  struct RigidBody
  {
  RigidBody()
	: force{Vector2Zero()}, velocity{Vector2Zero()},
	  mass{1.0f}, staticFriction{0}, dynamicFriction{0}
	{};
  RigidBody(Vector2 f, Vector2 v, float m, float sf, float df)
	: force{f}, velocity{v},
	  mass{m}, staticFriction{sf}, dynamicFriction{df}
	{};
	
	Vector2 force, velocity;
	float mass, staticFriction, dynamicFriction;
  };
  
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

  Rectangle ReCenter(const Rectangle&);
  Rectangle ReCenter(const adv::Transform&);
}
