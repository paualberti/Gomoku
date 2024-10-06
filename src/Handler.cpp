#include "Handler.hpp"
#include "Box.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

namespace std {
template <typename C, typename T> bool contains(C &&c, T e) {
  return std::find(std::begin(c), std::end(c), e) != std::end(c);
}
} // namespace std

Handler::Handler(const std::string title, const int size_window)
    : side(size_window / SIZE) {
  srand(time(NULL));
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << "Not properly initialized";
    throw SDL_GetError();
  }
  const int size = size_window + 2 * side;
  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, size, size, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer || !window) {
    std::cout << "Not properly initialized";
    throw SDL_GetError();
  }

  for (int i = 0; i < SIZE * SIZE; ++i) {
    grid.at(i) = new Box(renderer, i, side);
  }
  const int new_SIZE = SIZE + 2;
  for (int i = 0; i < new_SIZE * new_SIZE; ++i) {
    if (i < SIZE + 2 || i > new_SIZE * new_SIZE - new_SIZE ||
        i % new_SIZE == 0 || (i - new_SIZE + 1) % new_SIZE == 0) {
      jump.push_back(i);
    }
  }
  const int mark = jump.back();
  for (int i = 1; i < new_SIZE + 2; ++i) {
    jump.push_back(mark + i);
  }

  double part = 0.5;
  const int lat = side * part;
  const int pad = (side - lat) / 2;
  corners.at(0) = {size - pad - lat, pad, lat, lat};
  corners.at(1) = {pad, size - pad - lat, lat, lat};
  corners.at(2) = {pad, pad, lat, lat};
  corners.at(3) = {size - pad - lat, size - pad - lat, lat, lat};

  running = true;
  game = new Game(&grid);
  game->process_move((SIZE * SIZE) / 2);
  game->reverse_black();
  game->process_move((SIZE * SIZE) / 2 + SIZE + 1);
  game->reverse_black();
  for (auto &box : grid) {
    box->set_color(0, 0, 0);
  }
}

Handler::~Handler() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  delete game;
  for (int i = 0; i < SIZE * SIZE; ++i) {
    delete grid.at(i);
  }
  SDL_Quit();
}

bool Handler::is_running() { return running; }

void Handler::update() {
  const auto end = std::chrono::steady_clock::now();
  const auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  if (elapsed >= 500) {
    if (game->get_black()) {
      if (game->get_decisive_result()) {
        bg_color = {255, 255, 255, 255};
      } else {
        bg_color = {0, 0, 0, 255};
      }
    } else {
      if (game->get_decisive_result()) {
        bg_color = {0, 0, 0, 255};
      } else {
        bg_color = {255, 255, 255, 255};
      }
    }
  }

  for (int i : game->get_moves_played()) {
    /*if (grid.at(i)->getBlack() == game->getBlack()) {*/
    if (!game->get_decisive_result()) {
      grid.at(i)->set_color(0, 0, 0);
      continue;
    }
    if (grid.at(i)->get_black() == game->get_black()) {
      grid.at(i)->set_color(0, 0, 0);
    }
  }
  grid.at(game->get_moves_played().back())->set_color(255, 0, 0);
}

void Handler::render() {
  SDL_SetRenderDrawColor(renderer, 190, 151, 91, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int i = 1; i <= SIZE; ++i) {
    const int var_init = side / 2 + i * side;
    const int const_init = side / 2 + side;
    SDL_RenderDrawLine(renderer, var_init, const_init, var_init,
                       side * (SIZE + 2) - const_init); // Vertical lines
    SDL_RenderDrawLine(renderer, const_init, var_init,
                       side * (SIZE + 2) - const_init,
                       var_init); // Horizontal lines
  }

  /*for (int i : game->get_moves_played()) {*/
  /*  grid.at(i)->render();*/
  /*}*/
  for (int i = 0; i < SIZE * SIZE; ++i) {
    grid.at(i)->render();
  }
  for (SDL_Rect rect : corners) {
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b,
                           bg_color.a);
    SDL_RenderFillRect(renderer, &rect);
  }
  SDL_RenderPresent(renderer);
}

void Handler::events() {
  SDL_Event event;
  if (SDL_PollEvent(&event) == 0) {
    return;
  }
  switch (event.type) {
  case SDL_QUIT:
    running = false;
    break;

  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_q:
      running = false;
      break;

    case SDLK_h:
    case SDLK_LEFT:
      if (game->undo_last()) {
        game->reverse_black();
      }
      break;

    case SDLK_l:
    case SDLK_RIGHT:
      if (game->redo_last()) {
        game->reverse_black();
      }
      break;

    case SDLK_k:
    case SDLK_UP:
      while (game->undo_last()) {
        game->reverse_black();
        update();
      }
      break;

    case SDLK_j:
    case SDLK_DOWN:
      while (game->redo_last()) {
        game->reverse_black();
        update();
      }
      break;

    case SDLK_RETURN:
      while (play_engine_move() == 0) {
        update();
        render();
      }
      break;

    case SDLK_SPACE:
      play_engine_move();
    }
    break;

  case SDL_MOUSEBUTTONDOWN:
    Uint32 buttonState = SDL_GetMouseState(NULL, NULL);
    if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
      if (game->undo_last()) {
        game->reverse_black();
      }
    }

    if (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      const int x = event.button.x / side;
      const int y = event.button.y / side;
      int pos = y * (SIZE + 2) + x;
      if (std::contains(jump, pos)) {
        bg_color = {255, 0, 0, 255};
        start = std::chrono::steady_clock::now();
        break;
      }
      pos -= (SIZE + 3);
      pos -= pos / (SIZE + 2) * 2;

      if (game->process_move(pos)) {
        bg_color = {255, 0, 0, 255};
        start = std::chrono::steady_clock::now();
        break;
      }
      game->clear_undone();
      game->reverse_black();
      break;
    }
  }
}

int Handler::choose_engine_move() {
  int move = rand() % (SIZE * SIZE);
  for (int i = 0; i < SIZE * SIZE && grid.at(move)->get_clicked(); i++) {
    move = rand() % (SIZE * SIZE);
  }
  int priority = LIMIT;
  for (int i = 0; i < SIZE * SIZE; ++i) {
    if (grid.at(i)->get_clicked()) {
      continue;
    }
    if (priority > WIN_MOVE && game->has_winner(i)) {
      return i;
    }
    if (priority > TWO_FOUR && game->count_of_four(i) >= 2) {
      /*std::cout << "2four\n";*/
      priority = TWO_FOUR;
      move = i;
    }
    if (priority > TWO_THREE && game->count_of_three(i) >= 2) {
      /*std::cout << "2three\n";*/
      priority = TWO_THREE;
      move = i;
    }
    if (priority > THREE_FOUR && game->count_of_four(i) >= 1 &&
        game->count_of_three(i) >= 1) {
      /*std::cout << "three four\n";*/
      priority = THREE_FOUR;
      move = i;
    }
    if (priority > FOUR && game->count_of_four(i) >= 1) {
      /*std::cout << "three four\n";*/
      priority = FOUR;
      move = i;
    }

    game->reverse_black();
    if (priority > BLOCK_WIN_MOVE && game->has_winner(i)) {
      /*std::cout << "bwin\n";*/
      priority = BLOCK_WIN_MOVE;
      move = i;
    }
    if (priority > BLOCK_TWO_FOUR && game->count_of_four(i) >= 2) {
      /*std::cout << "b2four\n";*/
      priority = BLOCK_TWO_FOUR;
      move = i;
    }
    if (priority > BLOCK_TWO_THREE && game->count_of_three(i) >= 2) {
      /*std::cout << "b2three\n";*/
      priority = BLOCK_TWO_THREE;
      move = i;
    }
    if (priority > BLOCK_THREE_FOUR && game->count_of_four(i) >= 1 &&
        game->count_of_three(i) >= 1) {
      /*std::cout << "bthree four\n";*/
      priority = BLOCK_THREE_FOUR;
      move = i;
    }
    if (priority > BLOCK_ONE_FOUR && game->count_of_four(i) >= 1) {
      /*std::cout << "b1four\n";*/
      priority = BLOCK_ONE_FOUR;
      move = i;
    }
    if (priority > BLOCK_ONE_THREE && game->count_of_three(i) >= 1) {
      /*std::cout << "b1three\n";*/
      priority = BLOCK_ONE_THREE;
      move = i;
    }
    game->reverse_black();
  }
  return move;
}

int Handler::play_engine_move() {
  const int move = choose_engine_move();
  if (game->get_decisive_result()) {
    bg_color = {255, 0, 0, 255};
    start = std::chrono::steady_clock::now();
    return 1;
  }
  if (game->process_move(move)) {
    bg_color = {255, 0, 0, 255};
    start = std::chrono::steady_clock::now();
    return 1;
  }
  game->clear_undone();
  game->reverse_black();
  update();
  return 0;
}
