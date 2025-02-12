#pragma once

#define WALK_RIGHT_INDEX_BEGIN	0
#define WALK_RIGHT_INDEX_END	9
#define WALK_LEFT_INDEX_BEGIN	10
#define WALK_LEFT_INDEX_END		19
#define JUMP_INDEX_BEGIN		20
#define JUMP_INDEX_END		    29
#define IDLE_INDEX_BEGIN		22
#define IDLE_INDEX_END			27

#define WALK_RIGHT_ANIMATION				\
adv::Animation(								\
{											\
  {											\
	 .from		= WALK_RIGHT_INDEX_BEGIN,	\
	 .to		= WALK_RIGHT_INDEX_END,	    \
	 .duration	= .9f						\
  },										\
}											\
)											\

#define WALK_LEFT_ANIMATION				    \
adv::Animation(								\
{											\
  {											\
	 .from		= WALK_LEFT_INDEX_BEGIN,	\
	 .to		= WALK_LEFT_INDEX_END,	    \
	 .duration	= .9f						\
  },										\
}											\
)											\
 
#define JUMP_ANIMATION			            \
adv::Animation(					            \
{								            \
  {								            \
	 .from		= JUMP_INDEX_BEGIN,	        \
	 .to		= JUMP_INDEX_END,	        \
	 .duration	= .75f			            \
  },							            \
}								            \
)											\

#define IDLE_ANIMATION			            \
adv::Animation(					            \
{								            \
  {								            \
	 .from		= IDLE_INDEX_BEGIN,	        \
	 .to		= IDLE_INDEX_END,	        \
	 .duration	= 2.0f			            \
  },							            \
}								            \
)											
