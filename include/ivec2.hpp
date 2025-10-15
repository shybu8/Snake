#pragma once

#include <cstdint>
#include <raylib.h>

using std::int64_t;

struct IVec2 {
  int64_t x;
  int64_t y;

  IVec2();
  IVec2(int64_t value);
  IVec2(int64_t x, int64_t y);

  float len() const;

  bool operator==(const IVec2 &rhs) const noexcept;
  IVec2 operator*(const IVec2 &rhs) const noexcept;
  IVec2 operator+(const IVec2 &rhs) const noexcept;
  IVec2 operator-(const IVec2 &rhs) const noexcept;
  Vector2 raylib_vec() const noexcept;
};
