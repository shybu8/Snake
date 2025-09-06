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

  Vec2() {
    x = 0;
    y = 0;
  }

  Vec2(int64_t value) {
    x = value;
    y = value;
  }
  Vec2(int64_t x, int64_t y) {
    this->x = x;
    this->y = y;
  }

  uint64_t len() { return sqrt(x * x + y * y); };

  bool operator==(const Vec2 &rhs) const { return x == rhs.x and y == rhs.y; }
  Vec2 operator*(const Vec2 &rhs) const { return Vec2{x * rhs.x, y * rhs.y}; }
  Vec2 operator+(const Vec2 &rhs) const { return Vec2{x + rhs.x, y + rhs.y}; }
  Vec2 operator-(const Vec2 &rhs) const { return Vec2{x - rhs.x, y - rhs.y}; }
};

enum class Direction {
  Up = 0,
  Right,
  Down,
  Left,
};

enum class GameState { Ongoing, Over };

template <typename T> T mod(T lhs, T rhs);

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
  vector<Vec2> body_blocks;
  vector<SnakeTextureCell> texture_cells;
  float speed;
  Direction previous_direction;
  Direction direction;

  bool take_turn(Direction dir);
  Vec2 next_head_pos(Vec2 grid_dims);
  void advance(Vec2 grid_dims);
  bool collision(Vec2 target, bool include_head);
  float move_delay();
};
