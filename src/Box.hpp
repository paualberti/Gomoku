#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <string>
#define SIZE 15
#define TARGET 5

class Box {
public:
  Box(SDL_Renderer *renderer, const int pos, const int side);
  ~Box();

  SDL_Color get_color();
  bool get_black();
  bool get_clicked();
  int get_pos();
  void load_player(const bool black);
  void render();
  void set_black(const bool black);
  void set_clicked(const bool clicked);
  void set_color(const uint8_t r, const uint8_t g, const uint8_t b);

private:
  SDL_Color color;
  SDL_Rect box;
  int x, y;
  int rad1, rad2;
  SDL_Renderer *renderer;
  bool clicked;
  bool black;
  int pos;
};
