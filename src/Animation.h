#pragma once
#include <vector>
#include <iostream>
#include <raymath.h>
#include <cassert>

namespace adv
{
  struct Keyframe
  {
	float from, to, duration;
  };
  
  class Animation
  {
  public:
	Animation()
	  : m_Keyframes{{0.0f, 0.0f, 1.0f}}
	{}

	Animation(std::vector<Keyframe> kfs, bool)
	  : m_Keyframes{kfs}
	{}
	
	void Update(float dt)
	{
	  m_Index = m_Index % m_Keyframes.size();
	  m_Duration += dt;
	  
	  while (m_Duration >= m_Keyframes[m_Index].duration) {
        m_Duration -= m_Keyframes[m_Index].duration;
        m_Index = (m_Index + 1) % m_Keyframes.size();
	  }
	}

	float Value() const
	{
	  assert(!m_Keyframes.empty() && "No keyframes");
	  if (m_Index >= m_Keyframes.size())
		return m_Keyframes.back().to;

	  const Keyframe& kf = m_Keyframes[m_Index % m_Keyframes.size()];
	  return Lerp(kf.from, kf.to, m_Duration / kf.duration);
	}

	bool Empty() const
	{
	  return m_Keyframes.size() == 0 || m_Keyframes.empty();
	}

  private:
	std::vector<Keyframe> m_Keyframes {};
	size_t m_Index = 0;
	float m_Duration = 0.0f;
  };
}
