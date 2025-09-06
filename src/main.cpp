#include "structures.hpp"

#include <raylib.h>
#include <vector>

#include <fmt/base.h>
#include <fmt/format.h>

using std::string;
using std::uint64_t;
using std::vector;

void setRandomInDims(Vec2 &target, int64_t x, int64_t y) {
  target.x = GetRandomValue(0, x);
  target.y = GetRandomValue(0, y);
}

Vec2 rotated_texture_pos_adjustment(Direction orientation, Vec2 resolution) {
  using enum Direction;
  switch (orientation) {
  case Up:
    return Vec2{0, 0};
  case Right:
    return Vec2{resolution.x, 0};
  case Down:
    return Vec2{resolution.x, resolution.y};
  case Left:
    return Vec2{0, resolution.y};
  }
  return Vec2{};
}

float dir_to_angle(Direction dir) {
  using enum Direction;
  switch (dir) {
  case Up:
    return 0;
  case Right:
    return 90;
  case Down:
    return 180;
  case Left:
    return 270;
  }
  return 0;
}

int main() {
  const Vec2 screen{1200, 720};
  const Color white{255, 255, 255, 255};
  const Color black{0, 0, 0, 255};

  InitWindow(screen.x, screen.y, "Snake");
  SetTargetFPS(60);

  const int64_t grid_square_size{64};
  const float texture_scale = grid_square_size / 32.0;
  const Vec2 grid_dims = {screen.x / grid_square_size,
                          screen.y / grid_square_size};
  const float blinkng_delay = 0.5;
  Vec2 food = {};
  setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);

  Snake snake = {
      .body_blocks = vector<Vec2>{{1, 0}, {0, 0}},
      .texture_cells =
          vector<SnakeTextureCell>{
              {
                  Direction::Right,
                  SnakeTextureKind::Head,
              },
              {
                  Direction::Right,
                  SnakeTextureKind::Tail,
              },
          },
      .speed = 3,
      .previous_direction = Direction::Right,
      .direction = Direction::Right,
  };

  Texture2D snake_textures[4] = {
      LoadTexture("resources/snake_straight.png"),
      LoadTexture("resources/snake_turn.png"),
      LoadTexture("resources/snake_head.png"),
      LoadTexture("resources/snake_tail.png"),
  };
  for (auto texture : snake_textures)
    SetTextureFilter(texture, 0);

  Texture2D apple_texture = LoadTexture("resources/apple.png");
  SetTextureFilter(apple_texture, 0);

  Direction turn_dir = Direction::Right;
  float snake_delay{snake.move_delay()};
  string score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
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
          snake.body_blocks.push_back({});
          snake.texture_cells.push_back({});
          snake.speed += 0.1;

          do
            setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);
          while (snake.collision(food, true));

          snake_delay = snake.move_delay();
          score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
        }

        snake.advance(grid_dims);

        if (snake.collision(next_head_pos, false))
          game_state = GameState::Over;

        elapsed = 0;
      }

      BeginDrawing();
      {
        ClearBackground(black);
        DrawTextureEx(apple_texture,
                      {food.x * grid_square_size, food.y * grid_square_size}, 0,
                      texture_scale, white);
        // DrawRectangle(food.x * grid_square_size, food.y * grid_square_size,
        //               grid_square_size, grid_square_size, white);
        // for (Vec2 pos : snake.body_blocks)
        //   DrawRectangle(pos.x * grid_square_size, pos.y * grid_square_size,
        //                 grid_square_size, grid_square_size, white);
        for (uint64_t i = 0; i < snake.body_blocks.size(); ++i) {
          auto cell = snake.texture_cells.at(i);
          Texture2D texture = snake_textures[static_cast<int>(cell.kind)];
          Vec2 pos = snake.body_blocks.at(i) * Vec2(grid_square_size) +
                     rotated_texture_pos_adjustment(cell.orientation,
                                                    Vec2(grid_square_size));
          DrawTextureEx(
              texture,
              Vector2{static_cast<float>(pos.x), static_cast<float>(pos.y)},
              dir_to_angle(snake.texture_cells.at(i).orientation),
              texture_scale, white);
        }
        DrawText(score_str.c_str(), 0, 0, 28, WHITE);
        // DrawTextureEx(snake_straight_texture, {0, 0}, 0, texture_scale,
        // white);
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
        for (Vec2 pos : snake.body_blocks)
          DrawRectangle(pos.x * grid_square_size, pos.y * grid_square_size,
                        grid_square_size, grid_square_size, white);
      EndDrawing();

      break;
    }
  };

  CloseWindow();
}
