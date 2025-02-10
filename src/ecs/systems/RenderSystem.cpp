#include "RenderSystem.h"
#include "ecs/Registry.h"
#include <iostream>

extern Registry registry;

void RenderSystem::Update(float) 
{
  for (const Entity& e : m_Entities) {
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	DrawTexturePro(*s.GetTexture(), s.GetSource(),
				   adv::GetCenteredRect(t),
				   {0, 0}, 0, WHITE);
  }
}
