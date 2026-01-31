#include "Tetris.h"

int main() {
  Tetris tetris{{.start_w = 800, .start_h = 800}};
  tetris.Run();
  return 0;
}
