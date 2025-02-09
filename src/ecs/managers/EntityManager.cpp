#include "EntityManager.h"
#include <cassert>

EntityManager::EntityManager()
{
  for (Entity e = 0; e < MAX_ENTITIES; e++)
	m_Available.push(e);
}

Entity EntityManager::Create()
{
  assert(m_Alive < MAX_ENTITIES && "Too many alive entities"); 

  Entity e = m_Available.top();
  m_Available.pop();
  m_Alive++;
  return e;
}

void EntityManager::Destroy(Entity e)
{
  assert(e < MAX_ENTITIES && "Entity not created by manager, out of range");
  m_Signatures[e].reset();
  m_Available.push(e);
  m_Alive--;
}

void EntityManager::SetSignature(Entity e, Signature s)
{
  assert(e < MAX_ENTITIES && "Entity not created by manager, out of range");
  m_Signatures[e] = s;
}

Signature EntityManager::GetSignature(Entity e)
{
  assert(e < MAX_ENTITIES && "Entity not created by manager, out of range");
  return m_Signatures[e];
}
