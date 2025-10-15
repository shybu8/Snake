#include "grid.hpp"

Grid::Grid(uint64_t size, IVec2 offset) {
  this->size = size;
  this->offset = offset;
}

IVec2 Grid::at(IVec2 pos) {
  return IVec2(offset.x + size * pos.x, offset.y + size * pos.y);
}

IVec2 Grid::at(uint64_t x, uint64_t y) {
  float size_f = static_cast<float>(size);
  return IVec2(offset.x + size_f * x, offset.y + size_f * y);
}
