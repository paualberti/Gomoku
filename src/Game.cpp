#include "Game.hpp"
#include "Box.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <array>
#include <functional>
#include <vector>

Game::Game(std::array<Box *, SIZE * SIZE> *grid)
    : black(true), decisive_result(false), paint_win(true), grid(grid) {}

Game::~Game() {}

bool Game::get_black() { return black; }

bool Game::get_decisive_result() { return decisive_result; }

bool Game::has_winner(const int pos) {
  auto new_count = countAll(pos);
  for (Counter cnt : new_count) {
    if (cnt.cnt_2 >= TARGET) {
      return true;
    }
  }
  return false;
}

bool Game::process_move(const int pos) {
  if (decisive_result || grid->at(pos)->get_clicked()) {
    return true;
  }
  grid->at(pos)->load_player(black);
  movesPlayed.push_back(pos);
  decisive_result = has_winner(pos);
  return false;
}

bool Game::redo_last() {
  if (movesUndone.size() == 0) {
    return false;
  }
  const int last = movesUndone.back();
  grid->at(last)->set_clicked(true);
  movesPlayed.push_back(last);
  if (has_winner(last)) {
    decisive_result = true;
  }
  movesUndone.pop_back();
  return true;
}

bool Game::undo_last() {
  if (movesPlayed.size() <= 2) {
    return false;
  }
  decisive_result = false;
  const int last = movesPlayed.back();
  grid->at(last)->set_clicked(false);
  movesUndone.push_back(last);
  movesPlayed.pop_back();
  return true;
}

int Game::count_of_four(const int pos) {
  if (decisive_result || grid->at(pos)->get_clicked()) {
    return 0;
  }
  grid->at(pos)->set_clicked(true);
  grid->at(pos)->set_black(black);
  auto new_count = countAll(pos);
  int count = 0;
  int temp = 0;
  paint_win = false;
  for (Counter cnt : new_count) {
    if (cnt.cnt_2 >= 5) {
      continue;
    }
    if (cnt.b_1 >= 0 && has_winner(cnt.b_1)) {
      count++;
    }
    if (cnt.b_2 >= 0 && has_winner(cnt.b_2)) {
      count++;
    }
    temp++;
  }
  paint_win = true;
  grid->at(pos)->set_clicked(false);
  return count;
}

int Game::count_of_three(const int pos) {
  if (decisive_result || grid->at(pos)->get_clicked()) {
    return 0;
  }
  grid->at(pos)->set_clicked(true);
  grid->at(pos)->set_black(black);
  auto new_count = countAll(pos);
  int count = 0;
  for (Counter cnt : new_count) {
    paint_win = false;
    if ((cnt.b_2 >= 0 && count_of_four(cnt.b_2) >= 2) ||
        (cnt.b_1 >= 0 && count_of_four(cnt.b_1) >= 2)) {
      count++;
    }
    paint_win = true;
  }
  grid->at(pos)->set_clicked(false);
  return count;
}

std::vector<int> Game::get_moves_played() { return movesPlayed; }

void Game::clear_undone() { movesUndone.clear(); }

void Game::reverse_black() { black = !black; }

Counter Game::counter(const int pos, const int step, const SDL_Color color,
                      std::function<bool(int)> additionalCheck) {
  Counter count(0, 0);
  for (int i = 1; i < SIZE; ++i) {
    const int new_pos = pos + i * step;
    if (new_pos < 0 || new_pos >= SIZE * SIZE || !additionalCheck(new_pos)) {
      break;
    }

    if (!grid->at(new_pos)->get_clicked()) {
      if (count.b_1 >= 0) {
        break;
      }
      count.b_1 = new_pos;
    } else if (grid->at(new_pos)->get_black() == black) {
      count.cnt_1++;
      if (count.b_1 == -1) {
        count.cnt_2++;
        grid->at(new_pos)->set_color(color.r, color.g, color.b);
      }
    } else {
      break;
    }
  }
  return count;
}

Counter Game::count_dir(const int pos, const int step,
                        std::function<bool(int)> checkBoundary1,
                        std::function<bool(int)> checkBoundary2) {
  SDL_Color color = {0, 0, 0, 255};
  Counter count(1, 1);
  const auto count1 = counter(pos, -step, color, checkBoundary1);
  count.b_1 = count1.b_1;
  const auto count2 = counter(pos, step, color, checkBoundary2);
  count.b_2 = count2.b_1;
  /*Final sum calculation*/
  count.cnt_1 +=
      count1.cnt_2 + count2.cnt_2 +
      std::max(count1.cnt_1 - count1.cnt_2, count1.cnt_2 - count2.cnt_2);
  count.cnt_2 += count1.cnt_2 + count2.cnt_2;
  if (paint_win && count.cnt_2 >= TARGET) {
    color = {255, 0, 0, 255};
    counter(pos, -step, color, checkBoundary1);
    counter(pos, step, color, checkBoundary2);
  }
  return count;
}

std::array<Counter, 4> Game::countAll(const int pos) {
  return {count_dir(
              pos, SIZE, [this, pos](int p) { return p % SIZE == pos % SIZE; },
              [this, pos](int p) { return p % SIZE == pos % SIZE; }),
          count_dir(
              pos, 1, [this, pos](int p) { return p / SIZE == pos / SIZE; },
              [this, pos](int p) { return p / SIZE == pos / SIZE; }),
          count_dir(
              pos, SIZE + 1,
              [this, pos](int p) {
                return p % SIZE < pos % SIZE && p / SIZE < pos / SIZE;
              },
              [this, pos](int p) {
                return p % SIZE > pos % SIZE && p / SIZE > pos / SIZE;
              }),
          count_dir(
              pos, SIZE - 1,
              [this, pos](int p) {
                return p % SIZE > pos % SIZE && p / SIZE < pos / SIZE;
              },
              [this, pos](int p) {
                return p % SIZE < pos % SIZE && p / SIZE > pos / SIZE;
              })};
}
