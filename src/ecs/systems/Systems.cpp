#include "Systems.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "ecs/ECS.h"
#include "ecs/Registry.h"
#include "ecs/components/Components.h"
#include "util/Helper.h"

extern Registry registry;

void PhysicsSystem::Update(float dt)
{
  for (const Entity& e : m_Entities) 
	StepBody(
			 registry.GetComponent<adv::RigidBody>(e),
			 registry.GetComponent<adv::Transform>(e),
			 dt
			 );
  
}

void PhysicsSystem::StepBody(adv::RigidBody& r, adv::Transform& t, float dt)
{
  r.AddForce(G);
  r.ApplyForces(dt);
  t.Displace(r.velocity * dt);
  r.ResetForce();
}

void CollisionSystem::Update(float)
{
  std::vector<adv::Collision> collisions;
  
  for (const Entity& a : m_Entities) 
	for (const Entity& b : m_Entities) {
	  if (a == b) break;
	  adv::Collider& A = registry.GetComponent<adv::Collider>(a);
	  adv::Transform& at = registry.GetComponent<adv::Transform>(a);
	  adv::Collider& B = registry.GetComponent<adv::Collider>(b);
	  adv::Transform& bt = registry.GetComponent<adv::Transform>(b);
	  A.UpdateCenter(at.translation);
	  B.UpdateCenter(bt.translation);

	  adv::CollisionPoints points = adv::Collider::TestCollision(A, B);
	  
	  if (points.collided) 
		collisions.emplace_back(a, b, points);
	}

  ImpulseSolver(collisions);
  PositionSolver(collisions);
}

void PlayerUpdateSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	//	adv::Sprite& s = registry.GetComponent<adv::Sprite>(e);
	adv::RigidBody& r = registry.GetComponent<adv::RigidBody>(e);
	adv::Transform& t = registry.GetComponent<adv::Transform>(e);

	Vector2 step = {
	  (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A),
	  (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W)
	};
	r.AddForce(step * 100.0f);
  }
}

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

void RenderCollidersSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Collider& c = registry.GetComponent<adv::Collider>(e);
	DrawRectangleLinesEx(adv::ReCenter(c.rectangle),
	  2.0f, RED);
  }
}

