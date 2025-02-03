#include "ComponentManager.hpp"
#include <cassert>

template<typename T>
void ComponentManager::RegisterComponent()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) == m_ComponentIDs.end() && "Component allready registered");

  m_ComponentIDs.insert({ type, m_NextID++ });
  m_ComponentArrays.insert({ type, std::make_shared<ComponentArray<T>>() });
}
  
template<typename T>
ComponentID ComponentManager::GetComponentID()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");

  return m_ComponentIDs[type];
}
  
template<typename T>
void ComponentManager::AddComponent(Entity e, T c)
{
  ComponentManager::GetArray<T>()->Insert(e, c);
}

template<typename T>
void ComponentManager::RemoveComponent(Entity e)
{
  ComponentManager::GetArray<T>()->Remove(e);
}

  
template<typename T>
void ComponentManager::GetComponent(Entity e)
{
  return ComponentManager::GetArray<T>()->Get(e);
}

void ComponentManager::Destroy(Entity e)
{
  for (auto const& [type, arr] : m_ComponentArrays)
	arr->Destroy(e);
}

template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetArray()
{
  TypeID type = typeid(T).name();
  assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");
  return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[type]);
}

