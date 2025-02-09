#pragma once
#include "ecs/ECS.h"
#include "ecs/components/Sprite.h"
#include "ecs/components/Transform.h"
#include "util/Helper.h"

class RenderSystem : public System
{
public:
  void Update(float);
};
