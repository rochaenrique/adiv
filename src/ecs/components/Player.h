#pragma once
#include <cmath>

namespace adv
{
  enum PlayerState { WALK_LEFT, WALK_RIGHT, JUMP, IDLE };
  
  class Player
  {
  public:
	bool IsJumping() const { return m_State == JUMP; }
	void SetState(PlayerState s) { m_State = s; }
	PlayerState GetState() const { return m_State; }

	void SetStateFromStep(Vector2 step)
	{
	  if (step.x < 0.0f) SetState(PlayerState::WALK_LEFT);
	  else if (step.x > 0.0f) SetState(PlayerState::WALK_RIGHT);
	  else SetState(PlayerState::IDLE);
	  
	  float IsWorldUp(Vector2);
	  if (std::fabs(IsWorldUp({ 0.0f, step.y })) > 0.0f)
		SetState(PlayerState::JUMP);
	}

  private:
	PlayerState m_State = IDLE;
  };
}
