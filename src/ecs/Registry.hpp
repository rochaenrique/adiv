#pragma once
#include <memory>
#include "ECS.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"
#include "ComponentManager.hpp"

class Registry
{
public:
  void Init();
  
  Entity CreateEntity();
  
  void DestroyEntity(Entity);

  template<typename T>
  void RegisterComponent();

  template<typename T>
  void AddComponent(Entity, T);
  
  template<typename T>
  void RemoveComponent(Entity);

  template<typename T>
  T& GetComponent(Entity);

  template<typename T>
  ComponentID GetComponentID();
  
  template<typename T>
  std::shared_ptr<T> RegisterSystem();

  template<typename T>
  void SetSystemSignature(Signature);
  
private:
  std::unique_ptr<EntityManager>    m_EM;
  std::unique_ptr<SystemManager>    m_SM;
  std::unique_ptr<ComponentManager> m_CM;

  template<typename T>
  void UpdateSignature(Entity, bool);

};
