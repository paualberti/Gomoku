#include "circle.h"
#include <SDL2/SDL_render.h>

int SDL_RenderFillCircle(SDL_Renderer *renderer, const int x, const int y,
                         const int radius) {
  if (renderer == NULL || radius <= 0) {
    return -1; // Invalid renderer or radius
  }

  // Start at the top of the circle and move downwards
  for (int w = 0; w <= radius; w++) {
    int dx = w;
    int dy =
        (int)(sqrt(radius * radius - dx * dx)); // Using the circle equation

    // Draw horizontal lines using symmetry (top and bottom parts)
    SDL_RenderDrawLine(renderer, x - dy, y - dx, x + dy, y - dx); // Upper half
    SDL_RenderDrawLine(renderer, x - dy, y + dx, x + dy, y + dx); // Lower half
  }

  return 0; // Success
}
