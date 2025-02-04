#include "Registry.hpp"

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

void Registry::DestroyEntity(Entity e)
{
  m_EM->Destroy(e);   
  m_SM->Destroy(e);   
  m_CM->Destroy(e);	
}

template<typename T>
void Registry::RegisterComponent()
{
  m_CM->Register<T>();
};

template<typename T>
void Registry::AddComponent(Entity e, T c)
{
  m_CM->Add<T>(e, c);
  UpdateSignature<T>(e, true);
}

template<typename T>
void Registry::RemoveComponent(Entity e)
{
  m_CM->Remove<T>(e);
  UpdateSignature<T>(e, false);
}

template<typename T>
T& Registry::GetComponent(Entity e)
{
  return m_CM->Get<T>(e);
}

template<typename T>
ComponentID Registry::GetComponentID() {
  return m_CM->GetID<T>();
}
  
template<typename T>
std::shared_ptr<T> Registry::RegisterSystem()
{
  return m_SM->Register<T>();
}

template<typename T>
void Registry::SetSystemSignature(Signature s)
{
  m_SM->SetSignature<T>(s);
}

template<typename T>
void Registry::UpdateSignature(Entity e, bool b)
{
  Signature sig = m_EM->GetSignature(e);
  sig.set(m_CM->GetID<T>(), b);
  m_EM->SetSignature(e, sig);
  m_SM->SignatureChanged(e, sig);
}
