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
  void Add(Entity, T);

  template<typename T>
  void Remove(Entity);
  
  template<typename T>
  T& Get(Entity);

  void Destroy(Entity);

private:
  std::unordered_map<TypeID, ComponentID> m_ComponentIDs {};
  std::unordered_map<TypeID, std::shared_ptr<IComponentArray>> m_ComponentArrays {};

  ComponentID m_NextID;

  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetArray();
};
