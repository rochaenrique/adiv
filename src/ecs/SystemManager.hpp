#pragma once
#include "ECS.hpp"
#include <unordered_map>
#include <memory>

class SystemManager
{
public:
  template<typename T>
  std::shared_ptr<T> Register();

  template<typename T>
  void SetSignature(Signature);

  void Destroy(Entity e)
  {
	for (auto const& [type, sys] : m_Systems)
	  sys->m_Entities.erase(e);
  }
  
  void SignatureChanged(Entity, Signature);

private:
  std::unordered_map<TypeID, Signature> m_Signatures {};
  std::unordered_map<TypeID, std::shared_ptr<System>> m_Systems {};
};
