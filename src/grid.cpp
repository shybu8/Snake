#include "grid.hpp"

IVec2 Grid::at(IVec2 pos) { return IVec2(size * pos.x, size * pos.y); }

IVec2 Grid::at(uint64_t x, uint64_t y) {
  float size_f = static_cast<float>(size);
  return IVec2(size_f * x, size_f * y);
}
