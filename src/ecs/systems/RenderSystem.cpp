#include "RenderSystem.h"
#include "ecs/Registry.h"

extern Registry registry;

void RenderSystem::Update(float) 
{
  for (const Entity& e : m_Entities) {
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	DrawTexturePro(*s.texture, s.source,
				   adv::GetCenteredRect(t),
				   {0, 0}, 0, WHITE);
  }
}
