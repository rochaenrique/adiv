#pragma once
#include <bitset>
#include <cstdint>
#include <set>
#include <memory>

#define MAX_ENTITIES   200
#define MAX_COMPONENTS 16

using Entity      = std::uint16_t;
using Signature   = std::bitset<MAX_COMPONENTS>;
using ComponentID = std::uint8_t;
using TypeID      = const char *;

class System
{
public:
  void Update(float);
  std::set<Entity> m_Entities;
};



