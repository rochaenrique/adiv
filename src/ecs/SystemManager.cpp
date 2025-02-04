#include "SystemManager.hpp"
#include <cassert>

template<typename T>
std::shared_ptr<T> SystemManager::Register()
{
  TypeID type = typeid(T).name();
  assert(m_Systems.find(type) == m_Systems.end() && "Allready registered");
  std::shared_ptr<T> system = std::make_shared<T>();
  m_Systems.insert({ type, system });
  return system;
}

template<typename T>
void SystemManager::SetSignature(Signature s)
{
  TypeID type = typeid(T).name();
  assert(m_Signatures.find(type) != m_Signatures.end() && "Used before register");
  m_Signatures.insert({ type, s });
}

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

