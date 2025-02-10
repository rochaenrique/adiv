#pragma once
#include "ecs/ECS.h"
#include "ComponentArray.h"
#include <unordered_map>
#include <memory>
#include <cassert>

class ComponentManager
{
public:
  
  template<typename T>
  void Register()
  {
	TypeID type = typeid(T).name();
	assert(m_ComponentIDs.find(type) == m_ComponentIDs.end() && "Component allready registered");

	m_ComponentIDs.insert({ type, m_NextID++ });
	m_ComponentArrays.insert({ type, std::make_shared<ComponentArray<T>>() });
  }
  
  template<typename T>
  ComponentID GetID()
  {
	TypeID type = typeid(T).name();
	assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");
  
	return m_ComponentIDs[type];
  }
  
  template<typename T>
  void Add(Entity e, T c) { GetArray<T>()->Insert(e, c); }

  template<typename T>
  void Remove(Entity e) {GetArray<T>()->Remove(e); }
  
  template<typename T>
  T& Get(Entity e) { return GetArray<T>()->Get(e); }
  
  template<typename T>
  std::optional<T> RiskyGet(Entity e) { return GetArray<T>()->RiskyGet(e); }

  void Destroy(Entity);

private:
  std::unordered_map<TypeID, ComponentID> m_ComponentIDs {};
  std::unordered_map<TypeID, std::shared_ptr<IComponentArray>> m_ComponentArrays {};

  ComponentID m_NextID;

  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetArray()
  {
	TypeID type = typeid(T).name();
	assert(m_ComponentIDs.find(type) != m_ComponentIDs.end() && "Component not registered");
	return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[type]);
  }
};
