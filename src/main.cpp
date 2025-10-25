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

void set_random_in_dims(IVec2 &target, int64_t x, int64_t y) {
  target.x = GetRandomValue(0, x);
  target.y = GetRandomValue(0, y);
}

int main() {
  IVec2 screen{1280, 720};

  InitWindow(screen.x, screen.y, "Snake");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

  const int64_t true_texture_res = 32;
  const IVec2 grid_dims{static_cast<int>(16 * 1.5), static_cast<int>(9 * 1.5)};
  float grid_square_mul = static_cast<float>(screen.x) / grid_dims.x;
  auto grid = Grid(grid_square_mul, Vector2{0, 0});
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
  set_random_in_dims(food, grid_dims.x - 1, grid_dims.y - 1);

  Snake initial_snake = {
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
  Snake snake = initial_snake;

  float snake_delay{snake.move_delay()};
  string score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
  auto game_state = GameState::MainMenu;
  bool should_quit = WindowShouldClose();

  float elapsed = 0;
  while (!should_quit) {
    if (IsWindowResized()) {
      screen.x = GetScreenWidth();
      screen.y = GetScreenHeight();
      float grid_square_mul_x = static_cast<float>(screen.x) / grid_dims.x;
      float grid_square_mul_y = static_cast<float>(screen.y) / grid_dims.y;
      if (grid_square_mul_x < grid_square_mul_y) {
        grid_square_mul = grid_square_mul_x;
        grid.size = grid_square_mul;
        grid.offset.y = (screen.y - grid.size * grid_dims.y) / 2;
        grid.offset.x = 0;
      } else {
        grid_square_mul = grid_square_mul_y;
        grid.size = grid_square_mul;
        grid.offset.x = (screen.x - grid.size * grid_dims.x) / 2;
        grid.offset.y = 0;
      }
      texture_scale = static_cast<float>(grid.size) / true_texture_res;
    }

    switch (game_state) {

    case GameState::Ongoing:
      if (IsKeyDown(KEY_DOWN)) {
        snake.take_turn(Direction::Down);
      } else if (IsKeyDown(KEY_UP)) {
        snake.take_turn(Direction::Up);
      } else if (IsKeyDown(KEY_LEFT)) {
        snake.take_turn(Direction::Left);
      } else if (IsKeyDown(KEY_RIGHT)) {
        snake.take_turn(Direction::Right);
      }

      elapsed += GetFrameTime();
      if (elapsed > snake_delay) {
        IVec2 next_head_pos = snake.next_head_pos(grid_dims);
        if (next_head_pos == food) {
          snake.grow();
          snake.speed += 0.1;
          snake_delay = snake.move_delay();
          score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
        }

        snake.advance(grid_dims);

        if (next_head_pos == food)
          do
            set_random_in_dims(food, grid_dims.x - 1, grid_dims.y - 1);
          while (snake.collision(food));

        if (snake.collision(next_head_pos, false))
          game_state = GameState::Over;

        elapsed = 0;
      }

      BeginDrawing();
      {
        ClearBackground(BLACK);
        DrawTextureEx(background.texture, grid.at(0, 0), 0.0, texture_scale,
                      WHITE);
        DrawTextureEx(resources.apple, grid.at(food.x, food.y), 0,
                      texture_scale, WHITE);
        snake.draw(grid, resources, texture_scale);
        DrawText(score_str.c_str(), 0, 0, 28, WHITE);
      }
      EndDrawing();
      break;

    case GameState::Over:
      bool draw_snake;
      elapsed += GetFrameTime();
      if (elapsed < blinkng_delay)
        draw_snake = false;
      else if (elapsed > blinkng_delay * 2.0f)
        elapsed = 0;
      else
        draw_snake = true;

      BeginDrawing();
      ClearBackground(BLACK);
      DrawTextureEx(background.texture, grid.at(0, 0), 0.0, texture_scale,
                    WHITE);
      DrawText(score_str.c_str(), 0, 0, 28, WHITE);
      if (draw_snake)
        snake.draw(grid, resources, texture_scale);
      if (GuiButton(screen.relativeRect(.25f, .5f, .5f, .1f), "Retry")) {
        snake = initial_snake;
        snake_delay = snake.move_delay();
        score_str = fmt::format("Score: {}", snake.body_blocks.size() - 2);
        game_state = GameState::Ongoing;
      }

      if (GuiButton(screen.relativeRect(.25f, .7f, .5f, .1f), "Quit"))
        should_quit = true;
      EndDrawing();
      break;

    case GameState::MainMenu:
      BeginDrawing();
      ClearBackground(GRAY);
      GuiDrawText("Snake", screen.relativeRect(.4f, .25f, .2f, .1f),
                  TEXT_ALIGN_MIDDLE, WHITE);

      if (GuiButton(screen.relativeRect(.25f, .5f, .5f, .1f), "Play"))
        game_state = GameState::Ongoing;

      if (GuiButton(screen.relativeRect(.25f, .7f, .5f, .1f), "Quit"))
        should_quit = true;

      EndDrawing();
      break;
    }
    should_quit |= WindowShouldClose();
  };
  UnloadTexture(background.texture);
  CloseWindow();
}
