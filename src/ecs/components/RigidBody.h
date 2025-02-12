#pragma once
#include <raymath.h>
#include <iostream>

namespace adv
{
  class RigidBody
  {
  public:
  RigidBody()
	: m_Force{Vector2Zero()}, m_Velocity{Vector2Zero()},
	  m_Mass{1.0f}, m_StaticFriction{0}, m_DynamicFriction{0}, m_Restitution{1.0f},
	  m_Dynamic{true}
	{};
	
	RigidBody(Vector2 f, Vector2 v, float m, float sf, float df, float r, bool d)
	: m_Force{f}, m_Velocity{v},
	  m_Mass{m}, m_StaticFriction{sf}, m_DynamicFriction{df}, m_Restitution{r},
	  m_Dynamic{d}
	{};

	static RigidBody CreateStatic(float sf, float df)
	{
	  return RigidBody(Vector2Zero(), Vector2Zero(), 0.0f, sf, df, 1.0f, false);
	};

	void ApplyForce(Vector2 f)
	{
	  m_Force += f;
	};
	void ApplyAcc(Vector2 a)
	{
	  m_Force += a * m_Mass;
	};
	void ResolveForces(float dt)
	{
	  m_Velocity += m_Force * InvMass() * dt;
	}
	void ResetForce()
	{
	  lastForce = m_Force;
	  m_Force = Vector2Zero();
	};

	void ApplyVelocity(Vector2 v)
	{
	  m_Velocity += v;
	};
	
	Vector2 GetForce() const { return m_Force; }	
	Vector2 GetVelocity() const { return m_Velocity; }
	Vector2 GetLastForce() const { return lastForce; }

	float GetMass() const { return m_Mass; }
	float InvMass() const { return (m_Mass != 0.0f) ? (1.0f / m_Mass) : 0.0f; }

	float GetStaticFriction() const { return m_StaticFriction; }
	float GetDynamicFriction() const { return m_DynamicFriction; }
	float GetRestitution() const { return m_Restitution; }

	bool IsDynamic() { return m_Dynamic; };
  private:	
	Vector2 m_Force, m_Velocity, lastForce = Vector2Zero();
	float m_Mass, m_StaticFriction, m_DynamicFriction, m_Restitution;
	bool m_Dynamic;
  };
}
