#pragma once

#include <cstdint>
#include <ivec2.hpp>

using std::uint64_t;

struct Grid {
  uint64_t size;
  IVec2 offset;

  Grid(uint64_t size, IVec2 offset);

  IVec2 at(uint64_t x, uint64_t y);
  IVec2 at(IVec2 pos);
};
