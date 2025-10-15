#pragma once

#include <ivec2.hpp>
#include <vector>

using std::vector;

auto mod(auto lhs, auto rhs);

enum class Direction {
  Up = 0,
  Right,
  Down,
  Left,
};

enum class GameState { Ongoing, Over, MainMenu };

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
