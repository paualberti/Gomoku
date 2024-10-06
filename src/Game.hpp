#pragma once

#include "Box.hpp"
#include <SDL2/SDL_render.h>
#include <array>
#include <functional>
#include <vector>

struct Counter {
  Counter(const int cnt_1, const int cnt_2)
      : cnt_1(cnt_1), cnt_2(cnt_2), b_1(-1), b_2(-1) {}
  int cnt_1, cnt_2;
  int b_1, b_2;
};

class Game {
public:
  Game(std::array<Box *, SIZE * SIZE> *grid);
  ~Game();

  bool get_black();
  bool get_decisive_result();
  bool has_winner(const int pos);
  bool process_move(const int pos);
  bool redo_last();
  bool undo_last();
  int count_of_four(const int pos);
  int count_of_three(const int pos);
  std::vector<int> get_moves_played();
  void clear_undone();
  void reverse_black();

private:
  Counter count_dir(const int pos, const int step,
                   std::function<bool(int)> checkBoundary1,
                   std::function<bool(int)> checkBoundary2);
  Counter counter(
      const int pos, const int step, const SDL_Color color,
      std::function<bool(int)> additionalCheck = [](int) { return true; });
  std::array<Counter, 4> countAll(const int pos);

  bool black;
  bool decisive_result;
  bool paint_win;
  std::array<Box *, SIZE * SIZE> *grid;
  std::vector<int> movesPlayed;
  std::vector<int> movesUndone;
};
