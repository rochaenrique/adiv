#include "CameraSystem.h"
#include "ecs/Registry.h"
#include <raylib.h>
#include <raymath.h>
#include "ecs/components/Camera.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"
#include "Window.h"
#include <iostream>

extern Registry registry;

void CameraSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Camera& cc = registry.GetComponent<adv::Camera>(e);
	cc.camera->target = Vector2Zero();

	// adv::Transform& tc = registry.GetComponent<adv::Transform>(e);
	
	// cc.camera->target = tc.translation;
	// cc.camera->offset = Window::GetCenter();
  }
}
