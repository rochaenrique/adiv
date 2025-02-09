#include "RenderCollidersSystem.h"
#include "ecs/Registry.h"
#include "util/Helper.h"

extern Registry registry;

void RenderCollidersSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Collider& c = registry.GetComponent<adv::Collider>(e);
	DrawRectangleLinesEx(adv::ReCenter(c.rectangle),
	  2.0f, RED);
  }
}
