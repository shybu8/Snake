#pragma once

#include <grid.hpp>
#include <ivec2.hpp>
#include <resources.hpp>
#include <vector>

using std::vector;

auto mod(auto lhs, auto rhs);

enum class Direction {
  Up = 0,
  Right,
  Down,
  Left,
};

Vector2 rotated_texture_pos_adjustment(Direction orientation,
                                       const Vector2 &resolution);

float dir_to_angle(Direction dir);

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
  IVec2 next_head_pos(const IVec2 &grid_dims) const;
  void advance(const IVec2 &grid_dims);
  bool collision(const IVec2 &target, bool include_head = true) const;
  float move_delay() const;
  void draw(const Grid &grid_dims, const Resources &resources,
            const float &texture_scale) const;
  void grow();
};
