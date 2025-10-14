#pragma once

#include <structures.hpp>

#include <raylib.h>

struct Resources {
  Texture2D grass;
  Texture2D snake[4];
  Texture2D apple;

  Resources();
};
