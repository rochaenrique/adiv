#pragma once
#include <vector>
#include <iostream>
#include <raymath.h>

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
	  : m_Keyframes{}, m_Loop{false}
	{}

	Animation(std::initializer_list<Keyframe> kfs, bool l)
	  : m_Keyframes{kfs}, m_Loop{l}
	{}
	
	void Update(float dt)
	{
	  if (m_Index >= m_Keyframes.size()) return;
	  m_Duration += dt;

	  Keyframe& kf = m_Keyframes[m_Index];
	  while (m_Duration >= kf.duration && m_Index+1 < m_Keyframes.size()) {
		m_Index++;
		m_Duration -= kf.duration;
		kf = m_Keyframes[m_Index];
	  }

	  if (m_Loop && m_Index >= m_Keyframes.size())
		m_Index = 0;
	}

	float Value() const
	{
	  if (m_Index >= m_Keyframes.size())
		return m_Keyframes.back().to;

	  const Keyframe& kf = m_Keyframes[m_Index];
	  return Lerp(kf.from, kf.to, m_Duration / kf.duration);
	}

  private:
	std::vector<Keyframe> m_Keyframes {};
	bool m_Loop;
	size_t m_Index = 0;
	float m_Duration = 0.0f;
  };
}
