#include "Box.hpp"
#include "circle.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>

Box::Box(SDL_Renderer *renderer, const int pos, const int side)
    : renderer(renderer), clicked(false), black(false), pos(pos) {
  double part = 0.9;
  int new_side = side * part;
  int pad = (side - new_side) / 2;
  box = {side + pos % SIZE * side + pad, side + pos / SIZE * side + pad,
         new_side, new_side};
  rad1 = new_side / 2;
  part = 0.10;
  rad2 = side * part;
  this->x = side + pos % SIZE * side + side / 2;
  this->y = side + pos / SIZE * side + side / 2;
}

Box::~Box() {}

SDL_Color Box::get_color() { return color; }
bool Box::get_black() { return black; }
bool Box::get_clicked() { return clicked; }
int Box::get_pos() { return pos; }

void Box::load_player(const bool black) {
  this->clicked = true;
  this->black = black;
}

void Box::render() {
  if (!clicked) {
    return;
  }
  /*SDL_RenderCopy(renderer, player, nullptr, &box);*/
  if (black) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  }
  SDL_RenderFillCircle(renderer, x, y, rad1);
  if (color.r || color.g || color.b) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillCircle(renderer, x, y, rad2);
  }
}

void Box::set_black(const bool black) { this->black = black; };

void Box::set_clicked(const bool clicked) { this->clicked = clicked; }
void Box::set_color(const uint8_t r, const uint8_t g, const uint8_t b) {
  this->color = {r, g, b, 255};
}
