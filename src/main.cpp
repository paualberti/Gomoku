#include "Handler.hpp"

int main() {
  Handler *handler = new Handler("Gomoku", 714);
  while (handler->is_running()) {
    handler->events();
    handler->update();
    handler->render();
  }
  delete handler;
}
