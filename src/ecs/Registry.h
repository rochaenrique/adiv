#pragma once
#include <memory>
#include "ECS.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

class Registry
{
public:
  void Init();
  
  Entity CreateEntity();
  
  void DestroyEntity(Entity e);

  template<typename T>
  void RegisterComponent() { m_CM->Register<T>(); }

  template<typename T>
  void AddComponent(Entity e, T c)
  {
	m_CM->Add<T>(e, c);
	UpdateSignature<T>(e, true);
  }

  template<typename T>
  void RemoveComponent(Entity e)
  {
	m_CM->Remove<T>(e);
	UpdateSignature<T>(e, false);
  }

  template<typename T>
  T& GetComponent(Entity e) { return m_CM->Get<T>(e); }
  
  template<typename T>
  ComponentID GetComponentID() { return m_CM->GetID<T>(); }
  
  template<typename T>
  std::shared_ptr<T> RegisterSystem() { return m_SM->Register<T>(); }

  template<typename T>
  void SetSystemSignature(Signature s) { m_SM->SetSignature<T>(s); }
  
private:
  std::unique_ptr<EntityManager>    m_EM;
  std::unique_ptr<SystemManager>    m_SM;
  std::unique_ptr<ComponentManager> m_CM;

  template<typename T>
  void UpdateSignature(Entity e, bool b)
  {
	Signature sig = m_EM->GetSignature(e);
	sig.set(m_CM->GetID<T>(), b);
	m_EM->SetSignature(e, sig);
	m_SM->SignatureChanged(e, sig);
  }

};

