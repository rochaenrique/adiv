#pragma once
#include "Animation.h"
#include <unordered_map>
#include <cassert>
#include <iostream>

namespace adv {
  class Animator
  {
  public:
	Animator()
	  :	m_Current{0}
	{}

	void Insert(size_t i, const Animation& a) { m_Animations.emplace(i, a); };
	void Update(float dt)
	{
	  assert(m_Animations.find(m_Current) != m_Animations.end() && "No current animation");
	  m_Animations.at(m_Current).Update(dt);
	  std::cout << "Updated: " << m_Animations[m_Current].Value() << '\n';
	};
	float Value() 
	{
	  assert(m_Animations.find(m_Current) != m_Animations.end() && "No current animation");
	  return m_Animations.at(m_Current).Value();
	};
	void ChangeTo(size_t i)
	{
	  assert(m_Animations.find(i) != m_Animations.end() && "Non existing animation");
	  m_Current = i;
	  std::cout << "Changing to #" << i << '\n';
	};
	Animation Current() { return m_Animations.at(m_Current); };
  private:
	std::unordered_map<size_t, Animation> m_Animations {};
	size_t m_Current;
  };
}
