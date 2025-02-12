#pragma once
#include "ecs/ECS.h"
#include <queue>
#include <array>

class EntityManager
{
public:
  EntityManager();

  Entity Create();
  void Destroy(Entity);

  void SetSignature(Entity, Signature);
  Signature GetSignature(Entity);
  
private:
  std::queue<Entity> m_Available {};
  std::array<Signature, MAX_ENTITIES> m_Signatures {};
  int m_Alive {};
};

