#pragma once
#include <raylib.h>
#include "ecs/Components.h"

namespace adv
{
  Vector2 RandomVector2(int xlbound, int xrbound, int ylbound, int yrbound)
  {
	return {
	  (float)GetRandomValue(xlbound, xrbound),
	  (float)GetRandomValue(ylbound, yrbound)
	};
  };

  Vector2 RandomVector2(int xbound, int ybound)
  {
	return {
	  (float)GetRandomValue(0, xbound),
	  (float)GetRandomValue(0, ybound)
	};
  };

  RigidBody RandomRigidBody()
  {
	return adv::RigidBody(
						  { RandomVector2(-10, 10, -10, 10) },
						  { RandomVector2(-20, 20, -30, 30) },
						  (float)GetRandomValue(1, 15),
						  (float)GetRandomValue(0, 5),
						  (float)GetRandomValue(0, 5),
						  1.0f,
						  true
						  );
  };


}
