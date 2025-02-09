#include "SystemManager.h"

void SystemManager::SignatureChanged(Entity e, Signature s)
{
  for (auto const& [type, sys] : m_Systems) {
	const Signature& sig = m_Signatures[type];
	if ((s & sig) == sig)
	  sys->m_Entities.insert(e);
	else
	  sys->m_Entities.erase(e);
  }
}

void SystemManager::Destroy(Entity e)
{
  for (auto const& [type, sys] : m_Systems)
	sys->m_Entities.erase(e);
}
