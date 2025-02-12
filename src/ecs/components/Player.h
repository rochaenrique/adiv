#pragma once
#include <iostream>

#define WALK_RIGHT_INDEX_BEGIN	0
#define WALK_RIGHT_INDEX_END	9
#define WALK_LEFT_INDEX_BEGIN	10
#define WALK_LEFT_INDEX_END		19
#define JUMP_INDEX				22
#define IDLE_INDEX				27

#define WALK_RIGHT_ANIMATION				\
adv::Animation(								\
{											\
  {											\
	 .from		= WALK_RIGHT_INDEX_BEGIN,	\
	 .to		= WALK_RIGHT_INDEX_END,	    \
	 .duration	= .5f						\
  },										\
}											\
)											\

#define WALK_LEFT_ANIMATION				    \
adv::Animation(								\
{											\
  {											\
	 .from		= WALK_LEFT_INDEX_BEGIN,	\
	 .to		= WALK_LEFT_INDEX_END,	    \
	 .duration	= .5f						\
  },										\
  {											\
	 .from		= WALK_LEFT_INDEX_BEGIN,	\
	 .to		= WALK_LEFT_INDEX_END,	    \
	 .duration	= .5f						\
  }										    \
}											\
)											\
 
#define JUMP_ANIMATION			            \
adv::Animation(					            \
{								            \
  {								            \
	 .from		= JUMP_INDEX,	            \
	 .to		= JUMP_INDEX,	            \
	 .duration	= 3.0f			            \
  },							            \
}								            \
)											\

#define IDLE_ANIMATION			            \
adv::Animation(					            \
{								            \
  {								            \
	 .from		= IDLE_INDEX,	            \
	 .to		= IDLE_INDEX,	            \
	 .duration	= .1f			            \
  },							            \
}								            \
)											\

namespace adv
{
  enum PlayerState { WALK_LEFT, WALK_RIGHT, JUMP, IDLE };
  
  class Player
  {
  public:
	void SetJumping(bool b) { m_Jumping = b; }
	bool IsJumping() const { return m_Jumping; }
	void SetState(PlayerState s) { m_State = s; }
	PlayerState GetState() const { return m_State; }
  private:
	bool m_Jumping = false;
	PlayerState m_State = IDLE;
  };
}
