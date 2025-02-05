#pragma once
#include <memory>

namespace adv
{ 
  struct Gravity   { Vector2 force; };
  struct RigidBody { Vector2 acceleration, velocity; };
  struct Transform { Vector2 translation, scale, rotation };
  struct Sprite    { Texture2D texture; Rectangle source; };
}

