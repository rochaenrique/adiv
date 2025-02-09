#pragma once
#include "ecs/ECS.h"
#include <unordered_map>
#include <memory>
#include <cassert>

class SystemManager
{
public:
  template<typename T>
  std::shared_ptr<T> Register()
  {
	TypeID type = typeid(T).name();
	assert(m_Systems.find(type) == m_Systems.end() && "Allready registered");
	auto system = std::make_shared<T>();
	m_Systems.insert({type, system});
	return system;
  }

  template<typename T>
  void SetSignature(Signature s)
  {
	TypeID type = typeid(T).name();
	assert(m_Systems.find(type) != m_Systems.end() && "Used before register");
	m_Signatures.insert({ type, s });
  }

  void SignatureChanged(Entity, Signature);

  void Destroy(Entity e);

private:
  std::unordered_map<TypeID, Signature> m_Signatures {};
  std::unordered_map<TypeID, std::shared_ptr<System>> m_Systems {};
};
