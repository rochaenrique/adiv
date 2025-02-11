#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

namespace adv { 
  struct Camera
  {
	Camera()
	  : camera{std::make_shared<Camera2D>(Vector2Zero(), Vector2Zero(), 0.0f, 1.0f)}
	{}
  
	std::shared_ptr<Camera2D> camera;
  };
}
