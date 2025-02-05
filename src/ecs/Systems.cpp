#include "Systems.h"
#include "Registry.h"
#include "Components.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	Gravity& g   = registry.GetComponent<Gravity>(e);
	RigidBody& r = registry.GetComponent<RigidBody>(e);  
	Transform& t = registry.GetComponent<Transform>(e);

	t.translation.x += r.velocity.x * dt;
	t.translation.y += r.velocity.y * dt;
	  
	r.velocity.x += g.force.x * dt;
	r.velocity.y += g.force.y * dt;
  }
}

void RenderSystem::Update()
{
  for (const Entity& e : m_Entities) {
	Transform& t = registry.GetComponent<Transform>(e);
	Square&    s = registry.GetComponent<Square>(e);
	DrawRectangleV(
				   { t.translation.x, t.translation.y },
				   { t.scale.x, t.scale.y },
				   s.color
				   );
  }
}
