#pragma once

#include "Game.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <array>
#include <chrono>
#include <vector>

enum {
  WIN_MOVE = 0,
  BLOCK_WIN_MOVE,
  TWO_FOUR,
  THREE_FOUR,
  BLOCK_TWO_FOUR,
  BLOCK_THREE_FOUR,
  TWO_THREE,
  BLOCK_TWO_THREE,
  BLOCK_ONE_FOUR,
  BLOCK_ONE_THREE,
  FOUR,
  LIMIT,
};

class Handler {
public:
  Handler(const std::string title, const int size_window);
  ~Handler();

  bool is_running();
  void update();
  void render();
  void events();

private:
  int choose_engine_move();
  int play_engine_move();

  Game *game;
  SDL_Color bg_color;
  std::array<SDL_Rect, 4> corners;
  SDL_Renderer *renderer;
  SDL_Window *window;
  bool running;
  int side;
  std::array<Box *, SIZE * SIZE> grid;
  std::vector<int> jump;
  std::chrono::steady_clock::time_point start;
};
