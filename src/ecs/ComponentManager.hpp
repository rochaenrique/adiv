#pragma once
#include "ECS.hpp"
#include "ComponentArray.hpp"
#include <unordered_map>
#include <memory>

class ComponentManager
{
public:
  template<typename T>
  void Register();
  
  template<typename T>
  ComponentID GetID();
  
  template<typename T>
  void Add(Entity e, T c) { GetArray<T>()->Insert(e, c); }

  template<typename T>
  void Remove(Entity e) { GetArray<T>()->Remove(e); }
  
  template<typename T>
  void Get(Entity e) { return GetArray<T>()->Get(e); }

  void Destroy(Entity e)
  {
	for (auto const& [type, arr] : m_ComponentArrays)
	  arr->Destroy(e);
  }

private:
  std::unordered_map<TypeID, ComponentID> m_ComponentIDs {};
  std::unordered_map<TypeID, std::shared_ptr<IComponentArray>> m_ComponentArrays {};

  ComponentID m_NextID;

  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetArray();
};
