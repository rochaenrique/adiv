#include "ComponentManager.h"

void ComponentManager::Destroy(Entity e)
{
  for (auto const& [type, arr] : m_ComponentArrays)
	arr->Destroy(e);
}
