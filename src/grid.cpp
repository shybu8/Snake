#include "grid.hpp"

Grid::Grid(float size, Vector2 offset) {
  this->size = size;
  this->offset = offset;
}

Vector2 Grid::at(IVec2 pos) {
  return Vector2(offset.x + size * pos.x, offset.y + size * pos.y);
}

Vector2 Grid::at(uint64_t x, uint64_t y) {
  return Vector2(offset.x + size * x, offset.y + size * y);
}
