#include <resources.hpp>
#include <snake.hpp>

#include <apple.h>
#include <grass.h>
#include <snake_head.h>
#include <snake_straight.h>
#include <snake_tail.h>
#include <snake_turn.h>

Resources::Resources() {
  Image apple_img{};
  apple_img.format = APPLE_FORMAT;
  apple_img.width = APPLE_WIDTH;
  apple_img.height = APPLE_HEIGHT;
  apple_img.data = APPLE_DATA;
  apple_img.mipmaps = 1;
  apple = LoadTextureFromImage(apple_img);
  SetTextureFilter(apple, 0);

  Image grass_img{};
  grass_img.format = GRASS_FORMAT;
  grass_img.width = GRASS_WIDTH;
  grass_img.height = GRASS_HEIGHT;
  grass_img.data = GRASS_DATA;
  grass_img.mipmaps = 1;
  grass = LoadTextureFromImage(grass_img);
  SetTextureFilter(grass, 0);

  Image snake_straight{};
  snake_straight.format = SNAKE_STRAIGHT_FORMAT;
  snake_straight.width = SNAKE_STRAIGHT_WIDTH;
  snake_straight.height = SNAKE_STRAIGHT_HEIGHT;
  snake_straight.data = SNAKE_STRAIGHT_DATA;
  snake_straight.mipmaps = 1;
  snake[static_cast<int>(SnakeTextureKind::Straight)] =
      LoadTextureFromImage(snake_straight);

  Image snake_head{};
  snake_head.format = SNAKE_HEAD_FORMAT;
  snake_head.width = SNAKE_HEAD_WIDTH;
  snake_head.height = SNAKE_HEAD_HEIGHT;
  snake_head.data = SNAKE_HEAD_DATA;
  snake_head.mipmaps = 1;
  snake[static_cast<int>(SnakeTextureKind::Head)] =
      LoadTextureFromImage(snake_head);

  Image snake_tail{};
  snake_tail.format = SNAKE_TAIL_FORMAT;
  snake_tail.width = SNAKE_TAIL_WIDTH;
  snake_tail.height = SNAKE_TAIL_HEIGHT;
  snake_tail.data = SNAKE_TAIL_DATA;
  snake_tail.mipmaps = 1;
  snake[static_cast<int>(SnakeTextureKind::Tail)] =
      LoadTextureFromImage(snake_tail);

  Image snake_turn{};
  snake_turn.format = SNAKE_TURN_FORMAT;
  snake_turn.width = SNAKE_TURN_WIDTH;
  snake_turn.height = SNAKE_TURN_HEIGHT;
  snake_turn.data = SNAKE_TURN_DATA;
  snake_turn.mipmaps = 1;
  snake[static_cast<int>(SnakeTextureKind::Turn)] =
      LoadTextureFromImage(snake_turn);

  SetTextureFilter(snake[0], 0);
  SetTextureFilter(snake[1], 0);
  SetTextureFilter(snake[2], 0);
  SetTextureFilter(snake[3], 0);
}
