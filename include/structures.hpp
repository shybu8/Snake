#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>

using std::int64_t;
using std::sqrt;
using std::uint64_t;
using std::unique_ptr;
using std::vector;

struct Vec2 {
  int64_t x;
  int64_t y;

  uint64_t len() { return sqrt(x * x + y * y); };

  bool operator==(const Vec2 &rhs) const { return x == rhs.x and y == rhs.y; }
};

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

enum class GameState { Ongoing, Over };

template <typename T> T mod(T lhs, T rhs);

struct Snake {
  vector<Vec2> body_blocks;
  float speed;
  Direction previous_direction;
  Direction direction;

  bool take_turn(Direction dir);
  Vec2 next_head_pos(Vec2 grid_dims);
  void advance(Vec2 grid_dims);
  bool collision(Vec2 target, bool include_head);
  float move_delay();
};
