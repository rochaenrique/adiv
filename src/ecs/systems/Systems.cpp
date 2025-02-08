#include "Systems.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "ecs/ECS.h"
#include "ecs/Registry.h"
#include "ecs/components/Components.h"
#include "ecs/components/Collider.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);  
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);

	r.force += G * r.mass;
	assert(r.mass != 0 && "Mass is 0");
	
	r.velocity += r.force / r.mass * dt;
	t.translation += r.velocity * dt;

	r.force = Vector2Zero();
  }
}

void CollisionSystem::Update(float)
{
  std::vector<adv::Collision> collisions;
  
  for (const Entity& a : m_Entities) 
	for (const Entity& b : m_Entities) {
	  if (a == b) break;
	  
	  adv::Collider& A = registry.GetComponent<adv::Collider>(a);
	  adv::Collider& B = registry.GetComponent<adv::Collider>(b);
	  adv::Transform& at = registry.GetComponent<adv::Transform>(a);
	  adv::Transform& bt = registry.GetComponent<adv::Transform>(b);
	  
	  adv::CollisionPoints points = TestCollision(A, at.translation, B, bt.translation);
	  
	  if (points.collided)
		collisions.emplace_back(a, b, points);
	}

  ImpulseSolver(collisions);
  PositionSolver(collisions);
}

void RenderSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	DrawTexturePro(*s.texture, s.source,
				   adv::ReCenter(t),
				   {0, 0}, 0, WHITE);
  }
}

void RenderCollidersSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Collider& c = registry.GetComponent<adv::Collider>(e);
	DrawRectangleLinesEx(adv::ReCenter(c.rectangle),
	  2.0f, RED);
  }
}

void PlayerUpdateSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) {
	//	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);
	// std::cout << "t: (" <<
	//   t.translation.x << ", " <<
	//   t.translation.y << ")\n";
	
	// key presses cause accelerations
	if (IsKeyDown(KEY_W)) r.force.y -= 1;
	if (IsKeyDown(KEY_S)) r.force.y += 1;
	if (IsKeyDown(KEY_A)) r.force.x -= 1;
	if (IsKeyDown(KEY_D)) r.force.x += 1;
	
	// update animation regarding with respect to velocity
	// size_t slot = s.GetSlotIndex();
	// if (r.velocity.x > 0) 
	//   if (slot <= 9) s.IncrementSlotIndex(1);
	//   else s.SetSlotIndex(0);
	// else 
	//   if (slot >= 10 && slot < 20) s.IncrementSlotIndex(-1);
	//   else s.SetSlotIndex(10);

	// if (IsKeyPressed(KEY_P))
	//   s.IncrementSlotIndex(1);
	// if (IsKeyPressed(KEY_M))
	//   s.IncrementSlotIndex(-1);
	
	// std::cout << "vx:" << r.velocity.x << ", " <<
	//   " i:" << slot << ", s: (" << s.source.x << ", " << s.source.y <<  ")\n";
  }
}
