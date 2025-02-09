#pragma once
#include <raymath.h>
#include "RigidBody.h"

namespace adv
{
  struct Transform
  {
  Transform(Vector2 t, Vector2 s, Vector2 r)
	: translation{t}, scale{s}, rotation{r}
	{};
  Transform()
	: translation{Vector2Zero()}, scale{Vector2Zero()}, rotation{Vector2Zero()}
	{};

	void Displace(Vector2 s) { translation += s; };
	Vector2 translation, scale, rotation;
  };
}
