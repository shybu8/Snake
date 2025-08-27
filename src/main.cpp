#include "structures.hpp"

#include <memory>
#include <raylib.h>
#include <vector>

#include <fmt/base.h>
#include <fmt/format.h>

using std::make_unique;
using std::string;
using std::uint64_t;
using std::vector;

void setRandomInDims(Vec2 &target, int64_t x, int64_t y) {
  target.x = GetRandomValue(0, x);
  target.y = GetRandomValue(0, y);
}

int main() {
  const Vec2 screen{800, 600};
  const Color white{255, 255, 255, 255};
  const Color black{0, 0, 0, 255};

  InitWindow(screen.x, screen.y, "Snake");
  SetTargetFPS(60);

  const int64_t grid_square_size{24};
  const Vec2 grid_dims = {screen.x / grid_square_size,
                          screen.y / grid_square_size};
  const float blinkng_delay = 0.5;
  Vec2 food = {};
  setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);

  auto snake_body = vector<Vec2>{{1, 0}, {0, 0}};
  Snake snake = {
      .body_blocks = make_unique<vector<Vec2>>(snake_body),
      .speed = 3,
      .previous_direction = Direction::Right,
      .direction = Direction::Right,
  };

  Direction turn_dir;
  float snake_delay{snake.move_delay()};
  string score_str = fmt::format("Score: {}", snake.body_blocks->size() - 2);
  auto game_state = GameState::Ongoing;

  float elapsed = 0;
  while (!WindowShouldClose()) {
    switch (game_state) {

    case GameState::Ongoing:
      if (IsKeyDown(KEY_DOWN))
        turn_dir = Direction::Down;
      else if (IsKeyDown(KEY_UP))
        turn_dir = Direction::Up;
      else if (IsKeyDown(KEY_LEFT))
        turn_dir = Direction::Left;
      else if (IsKeyDown(KEY_RIGHT))
        turn_dir = Direction::Right;
      snake.take_turn(turn_dir);

      elapsed += GetFrameTime();
      if (elapsed > snake_delay) {
        Vec2 next_head_pos = snake.next_head_pos(grid_dims);
        if (next_head_pos == food) {
          snake.body_blocks->push_back({});
          snake.speed += 0.1;

          do
            setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);
          while (snake.collision(food, true));

          snake_delay = snake.move_delay();
          score_str = fmt::format("Score: {}", snake.body_blocks->size() - 2);
        }

        snake.advance(grid_dims);

        if (snake.collision(next_head_pos, false))
          game_state = GameState::Over;

        elapsed = 0;
      }

      BeginDrawing();
      {
        ClearBackground(black);
        DrawText(score_str.c_str(), 0, 0, 28, WHITE);
        DrawRectangle(food.x * grid_square_size, food.y * grid_square_size,
                      grid_square_size, grid_square_size, white);
        for (Vec2 pos : *snake.body_blocks)
          DrawRectangle(pos.x * grid_square_size, pos.y * grid_square_size,
                        grid_square_size, grid_square_size, white);
      }
      EndDrawing();
      break;

    case GameState::Over:
      bool draw_snake;
      elapsed += GetFrameTime();
      if (elapsed < blinkng_delay)
        draw_snake = false;
      else if (elapsed > blinkng_delay * 2)
        elapsed = 0;
      else
        draw_snake = true;

      BeginDrawing();
      ClearBackground(black);
      DrawText(score_str.c_str(), 0, 0, 28, WHITE);
      if (draw_snake)
        for (Vec2 pos : *snake.body_blocks)
          DrawRectangle(pos.x * grid_square_size, pos.y * grid_square_size,
                        grid_square_size, grid_square_size, white);
      EndDrawing();

      break;
    }
  };

  CloseWindow();
}
