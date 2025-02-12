#include "SpriteAnimationSystem.h"
#include "ecs/Registry.h"
#include "ecs/components/Sprite.h"
#include "ecs/components/SpriteAnimation.h"


extern Registry registry;
void SpriteAnimationSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::SpriteAnimation& anim = registry.GetComponent<adv::SpriteAnimation>(e);
	adv::Sprite& sprite = registry.GetComponent<adv::Sprite>(e);

	anim.Update(dt);
	sprite.SetIndex(anim.Value());
  }
}
