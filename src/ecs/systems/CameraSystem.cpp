#include "CameraSystem.h"
#include "ecs/Registry.h"
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include "ecs/components/Camera.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"
#include "Window.h"

extern Registry registry;

void CameraSystem::Update(float)
{
  for (const Entity& e : m_Entities) {
	adv::Camera& cc = registry.GetComponent<adv::Camera>(e);
	adv::Transform& tc = registry.GetComponent<adv::Transform>(e);
	
	cc.camera->target = tc.translation;
	cc.camera->offset = Window::GetCenter();

	Vector2 min = GetWorldToScreen2D({ 0.0f, tc.translation.y }, *cc.camera);
	Vector2 max = GetWorldToScreen2D({ (float)cc.mapWidth, tc.translation.y }, *cc.camera);

	if (IsKeyPressed(KEY_DOWN))
	  cc.camera->offset.x -= 5;
	if (IsKeyPressed(KEY_UP))
	  cc.camera->offset.x += 5;

	if (max.x < Window::GetWidth())
	  cc.camera->offset.x += Window::GetWidth() - max.x;
	else if (min.x > 0)
	  cc.camera->offset.x -= min.x;
  }
}
