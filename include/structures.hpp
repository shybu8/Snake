#pragma once

#include <cmath>
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <vector>

using std::int64_t;
using std::sqrt;
using std::uint64_t;
using std::unique_ptr;
using std::vector;

struct IVec2 {
  int64_t x;
  int64_t y;

  IVec2() {
    x = 0;
    y = 0;
  }

  IVec2(int64_t value) {
    x = value;
    y = value;
  }
  IVec2(int64_t x, int64_t y) {
    this->x = x;
    this->y = y;
  }

  uint64_t len() { return sqrt(x * x + y * y); };

  bool operator==(const IVec2 &rhs) const { return x == rhs.x and y == rhs.y; }
  IVec2 operator*(const IVec2 &rhs) const {
    return IVec2{x * rhs.x, y * rhs.y};
  }
  IVec2 operator+(const IVec2 &rhs) const {
    return IVec2{x + rhs.x, y + rhs.y};
  }
  IVec2 operator-(const IVec2 &rhs) const {
    return IVec2{x - rhs.x, y - rhs.y};
  }
  Vector2 raylib_vec() const {
    return Vector2{static_cast<float>(x), static_cast<float>(y)};
  }
};

enum class Direction {
  Up = 0,
  Right,
  Down,
  Left,
};

enum class GameState { Ongoing, Over, MainMenu };

auto mod(auto lhs, auto rhs);

enum class SnakeTextureKind {
  Straight = 0,
  Turn,
  Head,
  Tail,
};

struct SnakeTextureCell {
  Direction orientation;
  SnakeTextureKind kind;
};

struct Snake {
  vector<IVec2> body_blocks;
  vector<SnakeTextureCell> texture_cells;
  float speed;
  Direction previous_direction;
  Direction direction;

  bool take_turn(Direction dir);
  IVec2 next_head_pos(IVec2 grid_dims);
  void advance(IVec2 grid_dims);
  bool collision(IVec2 target, bool include_head = true);
  float move_delay();
};

struct Grid {
  uint64_t size;

  Vector2 at(uint64_t x, uint64_t y);
  Vector2 at(IVec2 pos);
};
