#include "AnimatorSystem.h"
#include "ecs/components/Animator.h"
#include "ecs/Registry.h"

extern Registry registry;

void AnimatorSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::Animator& a = registry.GetComponent<adv::Animator>(e);
	a.Update(dt);
	std::cout << "Updating\n";
  }
}
