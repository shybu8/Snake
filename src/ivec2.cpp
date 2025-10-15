#include <cmath>
#include <ivec2.hpp>

using std::sqrt;

IVec2::IVec2() {
  x = 0;
  y = 0;
}

IVec2::IVec2(int64_t value) {
  x = value;
  y = value;
}

IVec2::IVec2(int64_t x, int64_t y) {
  this->x = x;
  this->y = y;
}

float IVec2::len() const { return sqrt(x * x + y * y); }

bool IVec2::operator==(const IVec2 &rhs) const noexcept {
  return x == rhs.x and y == rhs.y;
}

IVec2 IVec2::operator*(const IVec2 &rhs) const noexcept {
  return IVec2{x * rhs.x, y * rhs.y};
}
IVec2 IVec2::operator+(const IVec2 &rhs) const noexcept {
  return IVec2{x + rhs.x, y + rhs.y};
}
IVec2 IVec2::operator-(const IVec2 &rhs) const noexcept {
  return IVec2{x - rhs.x, y - rhs.y};
}

Vector2 IVec2::raylib_vec() const noexcept {
  return Vector2{static_cast<float>(x), static_cast<float>(y)};
}
