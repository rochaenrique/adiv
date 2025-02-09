#pragma once
#include <memory>
#include "ECS.h"
#include "managers/SystemManager.h"
#include "managers/EntityManager.h"
#include "managers/ComponentManager.h"



class Registry
{
public:
  void Init();
  
  Entity CreateEntity();
  
  void DestroyEntity(Entity e);

  template<typename T>
  void RegisterComponent() { m_CM->Register<T>(); }

  template<typename Head, typename ... Tail>
  void RegisterMulComponent()
  {
	m_CM->Register<Head>();
	if constexpr(sizeof...(Tail) > 0)
	  RegisterMulComponent<Tail...>();
  }
  
  template<typename T>
  void AddComponent(Entity e, T c)
  {
	m_CM->Add<T>(e, c);
	UpdateSignature<T>(e, true);
  }

  template<typename Head, typename ... Tail>
  void AddMulComponent(Entity e, Head head, Tail... tail)
  {
	m_CM->Add<Head>(e, head);
	UpdateSignature<Head>(e, true);
	if constexpr(sizeof...(tail) > 0)
	  AddMulComponent(e, tail...);
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
  
  template<typename S, typename Head, typename ... Tail>
  std::shared_ptr<S> CreateSystem()
  {
	auto sys = RegisterSystem<S>();
	Signature sign;
	SignSystems<Head, Tail...>(sign);
	SetSystemSignature<S>(sign);
	return sys;
  }
  
  template<typename Head, typename ... Tail>
  void SignSystems(Signature sign)
  {
	sign.set(GetComponentID<Head>());
	if constexpr(sizeof...(Tail) > 0)
	  SignSystems<Tail...>(sign);
  }
  
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

