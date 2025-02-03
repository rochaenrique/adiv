#pragma once
#include <raylib.h>

class Player
{
 public:
  Player()
	: center{0, 0} {};
  void SayHi() const;
 private:
  Vector2 center;
};
