#include "Registry.h"

void Registry::Init()
{
  m_EM = std::make_unique<EntityManager>();
  m_SM = std::make_unique<SystemManager>();
  m_CM = std::make_unique<ComponentManager>();
}

Entity Registry::CreateEntity()
{
  return m_EM->Create();
};
