#pragma once

#include <cstdint>
#include <ivec2.hpp>

using std::uint64_t;

struct Grid {
  float size;
  Vector2 offset;

  Grid(float size, Vector2 offset);

  Vector2 at(uint64_t x, uint64_t y);
  Vector2 at(IVec2 pos);
};
