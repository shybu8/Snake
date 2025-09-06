#include "structures.hpp"

template <typename T> T mod(T lhs, T rhs) { return (lhs % rhs + rhs) % rhs; };

bool Snake::take_turn(Direction dir) {
  using enum Direction;
  if (dir == Up && this->previous_direction != Down)
    direction = Up;
  else if (dir == Down && this->previous_direction != Up)
    direction = Down;
  else if (dir == Right && this->previous_direction != Left)
    direction = Right;
  else if (dir == Left && this->previous_direction != Right)
    direction = Left;
  else {
    return false;
  }
  return true;
}

Vec2 Snake::next_head_pos(Vec2 grid_dims) {
  Vec2 head_pos{this->body_blocks.at(0)};
  switch (this->direction) {
  case Direction::Up:
    head_pos.y = mod(head_pos.y - 1, grid_dims.y);
    break;
  case Direction::Down:
    head_pos.y = mod(head_pos.y + 1, grid_dims.y);
    break;
  case Direction::Left:
    head_pos.x = mod(head_pos.x - 1, grid_dims.x);
    break;
  case Direction::Right:
    head_pos.x = mod(head_pos.x + 1, grid_dims.x);
    break;
  }
  return head_pos;
}

void Snake::advance(Vec2 grid_dims) {
  for (int64_t i = this->body_blocks.size() - 1; i > 0; i--) {
    this->body_blocks.at(i) = this->body_blocks.at(i - 1);
  }
  for (int64_t i = this->body_blocks.size() - 1; i > 1; i--) {
    this->texture_cells.at(i) = this->texture_cells.at(i - 1);
  }
  this->body_blocks.at(0) = this->next_head_pos(grid_dims);

  // BEGIN Snake turns trick
  auto next_to_head = &texture_cells.at(1);
  if (previous_direction != direction) {
    int s = static_cast<int>(direction) - static_cast<int>(previous_direction);
    if (s == 1 or s == -3)
      s = 2;
    next_to_head->kind = SnakeTextureKind::Turn;
    next_to_head->orientation = static_cast<Direction>(
        mod((static_cast<int>(previous_direction) + s), 4));
  } else {
    next_to_head->kind = SnakeTextureKind::Straight;
    next_to_head->orientation = direction;
  }
  // END

  // Tail
  Vec2 pre_tail = this->body_blocks.at(this->body_blocks.size() - 2);
  Vec2 tail_sub = pre_tail - this->body_blocks.back();
  if (tail_sub.x == 1)
    texture_cells.back().orientation = Direction::Right;
  else if (tail_sub.x == -1)
    texture_cells.back().orientation = Direction::Left;
  else if (tail_sub.y == 1)
    texture_cells.back().orientation = Direction::Down;
  else
    texture_cells.back().orientation = Direction::Up;

  texture_cells.back().kind = SnakeTextureKind::Tail;

  this->previous_direction = this->direction;
  this->texture_cells.at(0).orientation = this->direction;
}

bool Snake::collision(Vec2 target, bool include_head) {
  for (uint64_t i = !include_head; i < this->body_blocks.size(); i++)
    if (this->body_blocks.at(i) == target)
      return true;
  return false;
}

float Snake::move_delay() { return 1 / this->speed; }
