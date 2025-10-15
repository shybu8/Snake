#include "grid.hpp"
#include "ivec2.hpp"
#include "resources.hpp"
#include "snake.hpp"

#include <raylib.h>
#include <vector>

#include <fmt/base.h>
#include <fmt/format.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using std::string;
using std::uint64_t;
using std::vector;

void setRandomInDims(IVec2 &target, int64_t x, int64_t y) {
  target.x = GetRandomValue(0, x);
  target.y = GetRandomValue(0, y);
}

IVec2 rotated_texture_pos_adjustment(Direction orientation, IVec2 resolution) {
  using enum Direction;
  switch (orientation) {
  case Up:
    return IVec2{0, 0};
  case Right:
    return IVec2{resolution.x, 0};
  case Down:
    return IVec2{resolution.x, resolution.y};
  case Left:
    return IVec2{0, resolution.y};
  }
  return IVec2{};
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
  const IVec2 screen{1280, 720};

  InitWindow(screen.x, screen.y, "Snake");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

  const int64_t true_texture_res = 32;
  const IVec2 grid_dims{static_cast<int>(16 * 1.5), static_cast<int>(9 * 1.5)};
  float grid_square_mul = static_cast<float>(screen.x) / grid_dims.x;
  Grid grid{static_cast<uint64_t>(grid_square_mul)};
  float texture_scale = grid_square_mul / true_texture_res;

  const auto resources = Resources();

  RenderTexture2D background = LoadRenderTexture(
      true_texture_res * grid_dims.x, true_texture_res * grid_dims.y);
  BeginTextureMode(background);
  for (int i = 0; i < grid_dims.y; i++) {
    uint64_t y = i * true_texture_res;
    for (int j = 0; j < grid_dims.x; j++) {
      uint64_t x = j * true_texture_res;
      DrawTexture(resources.grass, x, y, WHITE);
    }
  }
  EndTextureMode();

  const float blinkng_delay = 0.5;
  IVec2 food = {};
  setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);

  Snake snake = {
      .body_blocks = vector<IVec2>{{1, 0}, {0, 0}},
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

  Direction turn_dir = Direction::Right;
  float snake_delay{snake.move_delay()};
  string score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
  auto game_state = GameState::MainMenu;
  bool should_quit = WindowShouldClose();

  float elapsed = 0;
  while (!should_quit) {
    if (IsWindowResized()) {
      grid_square_mul = static_cast<float>(GetScreenWidth()) / grid_dims.x;
      grid.size = static_cast<uint64_t>(grid_square_mul);
      // grid_square_size = static_cast<int>(grid_square_mul);
      texture_scale = grid_square_mul / true_texture_res;
    }

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
        IVec2 next_head_pos = snake.next_head_pos(grid_dims);
        if (next_head_pos == food) {
          snake.body_blocks.push_back({});
          snake.texture_cells.push_back({});
          snake.speed += 0.1;

          do
            setRandomInDims(food, grid_dims.x - 1, grid_dims.y - 1);
          while (snake.collision(food));

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
        ClearBackground(BLACK);
        DrawTextureEx(background.texture, {0, 0}, 0.0, texture_scale, WHITE);
        DrawTextureEx(resources.apple, grid.at(food.x, food.y).raylib_vec(), 0,
                      texture_scale, WHITE);
        for (uint64_t i = 0; i < snake.body_blocks.size(); ++i) {
          auto cell = snake.texture_cells.at(i);
          Texture2D texture = resources.snake[static_cast<int>(cell.kind)];
          IVec2 pos = snake.body_blocks.at(i) * IVec2(grid.size) +
                      rotated_texture_pos_adjustment(cell.orientation,
                                                     IVec2(grid.size));
          DrawTextureEx(texture, pos.raylib_vec(),
                        dir_to_angle(snake.texture_cells.at(i).orientation),
                        texture_scale, WHITE);
        }
        DrawText(score_str.c_str(), 0, 0, 28, WHITE);
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
      ClearBackground(BLACK);
      DrawTextureEx(background.texture, {0, 0}, 0.0, texture_scale, WHITE);
      DrawText(score_str.c_str(), 0, 0, 28, WHITE);
      if (draw_snake)
        for (IVec2 pos : snake.body_blocks)
          DrawRectangleV(grid.at(pos).raylib_vec(),
                         IVec2(grid.size, grid.size).raylib_vec(), WHITE);
      EndDrawing();
      break;

    case GameState::MainMenu:
      float scr_q = screen.x / 4;
      float height = 72;
      float pos_x = scr_q;
      float pos_y = screen.y / 6;
      BeginDrawing();
      ClearBackground(GRAY);
      GuiDrawText("Snake", {pos_x, pos_y, scr_q * 2, height}, TEXT_ALIGN_MIDDLE,
                  WHITE);
      if (GuiButton({pos_x, pos_y + 72 * 2, scr_q * 2, height}, "Play"))
        game_state = GameState::Ongoing;
      if (GuiButton({pos_x, pos_y + 72 * 4, scr_q * 2, height}, "Quit"))
        should_quit = true;

      EndDrawing();
      break;
    }
    should_quit |= WindowShouldClose();
  };
  UnloadTexture(background.texture);
  CloseWindow();
}
