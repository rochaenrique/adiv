#pragma once

#include <bitset>
#include <cstdint>
#include <set>

#define MAX_ENTITIES   1000
#define MAX_COMPONENTS 16

using Entity      = std::uint16_t;
using Signature   = std::bitset<MAX_COMPONENTS>;
using ComponentID = std::uint8_t;
using TypeID      = const char *;

class System
{
private:
  std::set<Entity> m_Entities;
};

