#include "controller.h"

void handleInput(GameInfo_t* info) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      info->action = LEFT;
      break;
    case KEY_RIGHT:
      info->action = RIGHT;
      break;
    case ' ':  // space
      info->action = ACTION;
      break;
    case KEY_DOWN:
      info->action = DOWN;
      break;
    case KEY_UP:
      info->action = UP;
      break;
    case '\n':  // enter
      info->action = START;
      break;
    case 'q':
      info->action = TERMINATE;
      break;
    case 'p':
      info->action = PAUSE;
      break;
  }
}