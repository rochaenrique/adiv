#pragma once
#include "ecs/ECS.h"
#include <array>
#include <unordered_map>
#include <cassert>
#include <optional>
#include <iostream>

class IComponentArray
{
public:
  virtual ~IComponentArray() = default;
  virtual void Destroy(Entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
  void Insert(Entity e, T c)
  {
	assert(m_Entity2Index.find(e) == m_Entity2Index.end() && "Allready added component to entity");

	size_t i = m_Size;
	m_Index2Entity[i] = e;
	m_Entity2Index[e] = i;
	m_Components[i] = c;
	m_Size++;
  }

  void Remove(Entity erm)
  {
	assert(m_Entity2Index.find(erm) != m_Entity2Index.end() && "Non-existent component");

	size_t irm = m_Entity2Index[erm];
	size_t ilast = m_Size - 1;
	m_Components[irm] = m_Components[ilast];

	Entity elast = m_Index2Entity[ilast];
	m_Entity2Index[elast] = irm;
	m_Index2Entity[irm] = elast;

	m_Entity2Index.erase(erm);
	m_Index2Entity.erase(irm);

	m_Size--;
  }

  T& Get(Entity e)
  {
	assert(m_Entity2Index.find(e) != m_Entity2Index.end() && "Non-existent component");
	return m_Components[m_Entity2Index[e]];
  }

  std::optional<T> RiskyGet(Entity e)
  {
	if (m_Entity2Index.find(e) == m_Entity2Index.end())
	  return {};
	return m_Components[m_Entity2Index[e]];
  }

  void Destroy(Entity e) override
  {
	if (e == 0) std::cout << "Found e\n";
	if (m_Entity2Index.find(e) != m_Entity2Index.end()) {
	  if (e == 0)
		std::cout << "Removing e\n";
	  Remove(e);
	}	  
	  
  }
  
private:
  std::array<T, MAX_ENTITIES> m_Components {};
  std::unordered_map<Entity, size_t> m_Entity2Index {};
  std::unordered_map<size_t, Entity> m_Index2Entity {};
  
  size_t m_Size;
};
  
