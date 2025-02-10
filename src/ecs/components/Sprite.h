#pragma once
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <iostream>
#include <cmath>

namespace adv
{
  class Sprite
  {
  public:
  Sprite()
	: m_Texture{nullptr}, m_Source{0, 0, 0, 0},
	  m_Index{0}, m_Grid{0, 0}
	{};
	Sprite(const std::shared_ptr<Texture2D>& t, Vector2 sz, size_t i)
	  : m_Texture{t}
	{
	  m_Source = { 0, 0, sz.x, sz.y };
	  m_Grid = { std::floor(t->width / sz.x), std::floor(t->height / sz.y) };
	  std::cout << "Grid is: (" << m_Grid.x << ", " << m_Grid.y << ")\n";
	  
	  if (IsInRange(i)) {
		m_Index = i;
		ReCalcSource();
	  } else 
		m_Index = 0;
	}

	bool SetIndex(size_t i)
	{
	  if (IsInRange(i)) {
		std::cout << "Setting index!\n";
		m_Index = i;
		ReCalcSource();
	  } else
		std::cout << "Not in range!\n";
	  return i == m_Index;
	}
	
	void IncrementIndex() { SetIndex(m_Index+1); }
	void DecrementIndex() { SetIndex(m_Index-1); }
	bool IsInRange(size_t lb, size_t rb) { return m_Index >= lb && m_Index <= rb; };
	void InRangeIncOrClamp(size_t lb, size_t rb)
	{
	  if (IsInRange(lb, rb)) IncrementIndex();
	  else SetIndex(lb);
	}
	void InRangeDecOrClamp(size_t lb, size_t rb)
	{
	  if (IsInRange(lb, rb)) DecrementIndex();
	  else SetIndex(lb);
	}

	const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }
	Rectangle GetSource() const { return m_Source; }
	size_t GetIndex() const { return m_Index; };
	
  private:
	std::shared_ptr<Texture2D> m_Texture;
	Rectangle m_Source;
	size_t m_Index;
	Vector2 m_Grid;

	bool IsInRange(size_t i) const
	{
	  std::cout << "Texting range\n";
	  return i < (m_Grid.x * m_Grid.y);
	};
	void ReCalcSource()
	{
	  m_Source.x = (m_Index % (size_t)m_Grid.x) * m_Source.width;
	  m_Source.y = (m_Index / (size_t)m_Grid.x) * m_Source.height;
	  std::cout << "Index -> " << m_Index << ", src -> (" <<
		m_Source.x << ", " << m_Source.y << ")\n";
	}
  };
}
