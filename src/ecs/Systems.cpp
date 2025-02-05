#include "Systems.h"
#include "Registry.h"
#include "Components.h"
#include <raylib.h>
#include <iostream>

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::Gravity& g = registry.GetComponent<adv::Gravity>(e);
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);  
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);

	t.translation.x += r.velocity.x * dt;
	t.translation.y += r.velocity.y * dt;
	  
	r.velocity.x += (g.force.x + r.acceleration.x) * dt;
	r.velocity.y += (g.force.y + r.acceleration.y) * dt;
  }
}

void RenderSystem::Update()
{
  for (const Entity& e : m_Entities) {
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	DrawTexturePro(*s.texture, s.source,
				   { t.translation.x, t.translation.y, t.scale.x, t.scale.y},
				   {0, 0}, 0, RED);
  }
}

void PlayerUpdateSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);

	// key presses cause accelerations
	if (IsKeyDown(KEY_W)) r.acceleration.y -= 1;
	if (IsKeyDown(KEY_S)) r.acceleration.y += 1;
	if (IsKeyDown(KEY_A)) r.acceleration.x -= 1;
	if (IsKeyDown(KEY_D)) r.acceleration.x += 1;
	
	// update animation regarding with respect to velocity
	size_t slot = s.GetSlotIndex();
	/*
	if (r.velocity.x > 0) 
	  if (slot <= 9) s.IncrementSlotIndex(1);
	  else s.SetSlotIndex(0);
	else 
	  if (slot >= 10 && slot < 20) s.IncrementSlotIndex(-1);
	  else s.SetSlotIndex(10);
	*/

	if (IsKeyPressed(KEY_P))
	  s.IncrementSlotIndex(1);
	if (IsKeyPressed(KEY_M))
	  s.IncrementSlotIndex(-1);

	
	std::cout << "vx:" << r.velocity.x << ", " <<
	  " i:" << slot << ", s: (" << s.source.x << ", " << s.source.y <<  ")\n";
  }
}
