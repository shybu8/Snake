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
  this->previous_direction = this->direction;
  for (int64_t i = this->body_blocks.size() - 1; i > 0; i--) {
    this->body_blocks.at(i) = this->body_blocks.at(i - 1);
  }
  this->body_blocks.at(0) = this->next_head_pos(grid_dims);
}

bool Snake::collision(Vec2 target, bool include_head) {
  for (uint64_t i = !include_head; i < this->body_blocks.size(); i++)
    if (this->body_blocks.at(i) == target)
      return true;
  return false;
}

float Snake::move_delay() { return 1 / this->speed; }
