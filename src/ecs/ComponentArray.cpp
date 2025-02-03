#include "ComponentArray.hpp"
#include <cassert>

template<typename T>
void ComponentArray<T>::Insert(Entity e, T c)
{
  assert(m_Entity2Index.find(e) == m_Entity2Index.end() && "Allready added component to entity");

  size_t i = m_Size;
  m_Index2Entity[i] = e;
  m_Entity2Index[e] = i;
  m_Components[i] = c;
  m_Size++;
}

template<typename T>
void ComponentArray<T>::Remove(Entity erm) 
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

template <typename T>
T& ComponentArray<T>::Get(Entity e)
{
  assert(m_Entity2Index.find(e) != m_Entity2Index.end() && "Non-existent component");
  return m_Components[m_Entity2Index[e]];
}

template<typename T>
void ComponentArray<T>::Destroy(Entity e)
{
  if (m_Entity2Index.find(e) != m_Entity2Index.end())
	ComponentArray::Remove(e);
}
