#pragma once
#include "ECS.hpp"
#include <array>
#include <unordered_map>

class IComponentArray
{
public:
  virtual ~IComponentArray() = default;
  virtual void Destroy(Entity) = 0;
};

template<typename T>
class ComponentArray : IComponentArray
{
public:
  void Insert(Entity, T);
  void Remove(Entity);
  T& Get(Entity);
  void Destroy(Entity);
  
private:
  std::array<T, MAX_ENTITIES> m_Components {};
  std::unordered_map<Entity, size_t> m_Entity2Index {};
  std::unordered_map<size_t, Entity> m_Index2Entity {};
  
  size_t m_Size;
};
  
