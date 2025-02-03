#pragma once
#include "ECS.hpp"
#include "ComponentManager.hpp"
#include <stack>
#include <array>

class EntityManager
{
public:
  EntityManager();

  Entity CreateEntity();
  void DestroyEntity(Entity);

  void SetSignature(Entity, Signature);
  Signature GetSignature(Entity);
  
private:
  std::stack<Entity> m_Available {};
  std::array<Signature, MAX_ENTITIES> m_Signatures {};
  int m_Alive {};
};

