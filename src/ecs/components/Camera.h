#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

namespace adv { 
  struct Camera
  {
	Camera()
	  : camera{std::make_shared<Camera2D>(Vector2Zero(), Vector2Zero(), 0.0f, .3f)}
	{}
	
	Camera(size_t width)
	  : camera{std::make_shared<Camera2D>(Vector2Zero(), Vector2Zero(), 0.0f, .3f)}, mapWidth{width}
	{}

  
	std::shared_ptr<Camera2D> camera;
	size_t mapWidth;
  };
}
