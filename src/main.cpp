#include "Handler.hpp"

int main() {
  Handler *handler = new Handler("Gomoku", 650);
  while (handler->is_running()) {
    handler->events();
    handler->update();
    handler->render();
  }
  delete handler;
}
