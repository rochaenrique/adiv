#pragma once
#include "Animation.h"
#include <cassert>
#include <iostream>
#include <unordered_map>

namespace adv
{
  class Animator
  {
  public:
	Animator() {}

	void Insert(size_t i, const Animation a)
	{
	  m_Animations[i] = std::make_shared<Animation>(a);
	}
	
	void Update(float dt)
	{
	  assert(m_Current != nullptr && "No current animation");
	  if (!m_Current->Empty()) m_Current->Update(dt);
	}
	
	float Value() 
	{
	  assert(m_Current != nullptr && "No current animation");
	  if (!m_Current) return 0.0f;
	  return m_Current->Value();
	}
	
	void ChangeTo(size_t i)
	{
	  auto next = m_Animations.find(i);
	  assert(next != m_Animations.end() && "Non existing animation");
	  m_Current = next->second;
	}
	
	std::shared_ptr<Animation> Current()
	{
	  assert(m_Current && "No current animation");
	  return m_Current;
	}
	
  private:
	std::unordered_map<size_t, std::shared_ptr<Animation>> m_Animations;
	std::shared_ptr<Animation> m_Current = nullptr;
  };
}
