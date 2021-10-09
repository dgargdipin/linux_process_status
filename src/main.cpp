#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  printf("\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n");
  printf("Created system\n");
  NCursesDisplay::Display(system);
}