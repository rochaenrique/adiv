#pragma once
#include <iostream>

namespace adv
{
  class Player
  {
  public:
	void SetJumping(bool b)
	{
	  jumping = b;
	  std::cout << "Set jumping to:" << b << '\n';
	};
	bool IsJumping() const { return jumping; }
  private:
	bool jumping = false;
  };
}
