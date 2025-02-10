#pragma once

namespace adv
{
  class Player
  {
  public:
	void SetJumping(bool b) { jumping = b; };
	bool IsJumping() const { return jumping; }
  private:
	bool jumping = false;
  };
}
