#include "ComponentManager.hpp"
#include <cassert>

template<typename T>
void ComponentManager::Register()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) == m_ComponentIDs.end() && "Component allready registered");

  m_ComponentIDs.insert({ type, m_NextID++ });
  m_ComponentArrays.insert({ type, std::make_shared<ComponentArray<T>>() });
}
  
template<typename T>
ComponentID ComponentManager::GetID()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");

  return m_ComponentIDs[type];
}
  
template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetArray()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");
  return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[type]);
}

