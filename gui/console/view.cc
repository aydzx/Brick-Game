#include "view.h"

void printGameField(WindowGame* gameWindow,GameInfo_t* data, GameState state) {
  werase(gameWindow->GameWindow);
  werase(gameWindow->NextFigure);
  werase(gameWindow->GameScore);
  box(gameWindow->NextFigure, 0, 0);
  box(gameWindow->GameScore, 0, 0);
  printInfo(gameWindow->GameScore, data);
  if (state != MENU && state != GAME_OVER ) {
    printNextFigure(gameWindow->NextFigure, data);
    printField(gameWindow->GameWindow, data);
  } else if (state == MENU) {
    printMenu(gameWindow->GameWindow);
  } else if (state == GAME_OVER) {
    printGameOver(gameWindow->GameWindow);
  }
  if (state == STOP) {
    printPause(gameWindow->GameScore);
  }
  wrefresh(gameWindow->GameWindow);
  wrefresh(gameWindow->NextFigure);
  wrefresh(gameWindow->GameScore);
  wrefresh(gameWindow->GameBorder);
}

void printMenu(WINDOW* GameWindow) {
  wattron(GameWindow, COLOR_PAIR(1));
  mvwprintw(GameWindow, 8, 4, "Press 'Enter'");
  mvwprintw(GameWindow, 10, 6, "to start");
  wattroff(GameWindow, COLOR_PAIR(1));
}

void InitWindow() {
  initscr();
  start_color();
  InitColor();
  noecho();
  nodelay(stdscr, true);
  cbreak();
  keypad(stdscr, 1);
  curs_set(0);
  timeout(10);
}
void InitColor() {
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void printInfo(WINDOW* GameScore, GameInfo_t* data) {
  wattron(GameScore, COLOR_PAIR(2));
  mvwprintw(GameScore, 5, 2, "Score: ");
  wattroff(GameScore, COLOR_PAIR(2));
  mvwprintw(GameScore, 5, 10, "%d ", data->score);
  wattron(GameScore, COLOR_PAIR(3));
  mvwprintw(GameScore, 7, 2, "Speed: ");
  wattroff(GameScore, COLOR_PAIR(3));
  mvwprintw(GameScore, 7, 10, "%d ", data->speed);
  wattron(GameScore, COLOR_PAIR(4));
  mvwprintw(GameScore, 9, 2, "Level: ");
  wattroff(GameScore, COLOR_PAIR(4));
  mvwprintw(GameScore, 9, 10, "%d ", data->level);
  wattron(GameScore, COLOR_PAIR(5));
  mvwprintw(GameScore, 11, 2, "HiScore: ");
  wattroff(GameScore, COLOR_PAIR(5));
  mvwprintw(GameScore, 11, 10, " %d ", data->high_score);
}

void printNextFigure(WINDOW* NextFigure, GameInfo_t* data) {
  for (int y = 0; y < 4; y++) {
    wattron(NextFigure, COLOR_PAIR(data->next.color));
    mvwaddch(NextFigure, data->next.pixels[y].y + OFFSET_Y_NEXT_SHAPE,
             data->next.pixels[y].x * 2 - OFFSET_Y_NEXT_SHAPE, BLOCK);
    mvwaddch(NextFigure, data->next.pixels[y].y + OFFSET_Y_NEXT_SHAPE,
             data->next.pixels[y].x * 2 - OFFSET_Y_NEXT_SHAPE + 1,
             BLOCK);
    wattroff(NextFigure, COLOR_PAIR(data->next.color));
  }
}

void printField(WINDOW* GameWindow, GameInfo_t* data) {
  for (int y = 3; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (data->field[y][x]) {
        wattron(GameWindow, COLOR_PAIR(data->field[y][x]));
        mvwaddch(GameWindow, y - OFFSET_Y , x * 2, BLOCK);
        mvwaddch(GameWindow, y - OFFSET_Y , x * 2 + 1, BLOCK);
        wattroff(GameWindow, COLOR_PAIR(data->field[y][x]));
      }
    }
  }
}

void initWindowGame(WindowGame* gameWindow) {
  gameWindow->GameWindow = newwin(HEIGHT_WINDOWS_GAME, WIDTH_WINDOWS_GAME,
                                  Y_WINDOWS_GAME, X_WINDOWS_GAME);
  gameWindow->GameBorder = newwin(HEIGHT_BORDER, WIDTH_BORDER,
                                  STARTING_COORDINATE, STARTING_COORDINATE);
  gameWindow->GameScore = newwin(HEIGHT_WINDOWS_INFO, WIDTH_WINDOWS_INFO,
                                 Y_WINDOWS_INFO, X_WINDOWS_INFO);
  gameWindow->NextFigure = newwin(HEIGHT_WINDOWS_NEXT, WIDTH_WINDOWS_NEXT,
                                  STARTING_COORDINATE, X_WINDOWS_INFO);
  box(gameWindow->GameBorder, 0, 0);
}

void printGameOver(WINDOW* GameWindow) {
  wattron(GameWindow, COLOR_PAIR(1));
  mvwprintw(GameWindow, 9, 2, "G A M E  O V E R");
  mvwprintw(GameWindow, 11, 4, "Press 'Enter'");
  mvwprintw(GameWindow, 12, 5, "to restart");
  wattroff(GameWindow, COLOR_PAIR(1));
}

void delitWindows(WindowGame* gameWindow) {
  delwin(gameWindow->GameWindow);
  delwin(gameWindow->GameBorder);
  delwin(gameWindow->NextFigure);
  delwin(gameWindow->GameScore);
  endwin();
}

void printPause(WINDOW* GameScore) {
  wattron(GameScore, COLOR_PAIR(1));
  mvwprintw(GameScore, 2, 2, "P A U S E");
  wattroff(GameScore, COLOR_PAIR(1));
}


double wtime() {
  double result = 0.0;
#ifdef CLOCK_MONOTONIC
  struct timespec ts = {.tv_sec = 0,.tv_nsec = 0  };
  clock_gettime(CLOCK_MONOTONIC, &ts);
  result = ts.tv_sec + (double)ts.tv_nsec * 1E-9;
#else
  result = (double)clock();
#endif
  return result;
}

double getCpuTime(double* lastIterMoment, double* currIterMoment) {
  double CpuTime = 0.0;  // Переменная для накопления времени
  *currIterMoment = wtime();  // Фиксируем время окончания текущей итерации
#ifdef CLOCK_MONOTONIC
  CpuTime = ((double)(*currIterMoment - *lastIterMoment));
#else
  CpuTime = ((double)(*currIterMoment - *lastIterMoment)) / CLOCKS_PER_SEC;
#endif
  return CpuTime;
}


int chooseGame(WindowGame* gameWindow) {
  int colorSnake = 7;
  int colorTetris = 1;
  int choose = 1;
  int ch = 0 ;
  while( ch != '\n') {
    ch = getch();
     mvwprintw(gameWindow->GameWindow, 5, 2, "Choose a game ");
     wattron(gameWindow->GameWindow, COLOR_PAIR(colorTetris));
     mvwprintw(gameWindow->GameWindow, 7, 2, "TETRIS");
     wattroff(gameWindow->GameWindow, COLOR_PAIR(colorTetris));
     wattron(gameWindow->GameWindow, COLOR_PAIR(colorSnake));
     mvwprintw(gameWindow->GameWindow, 9, 2, "SNAKE");
     wattroff(gameWindow->GameWindow, COLOR_PAIR(colorSnake));
     wrefresh(gameWindow->GameBorder);
      wrefresh(gameWindow->GameScore);
      wrefresh(gameWindow->GameWindow);
      wrefresh(gameWindow->NextFigure);
    if(choose == 1) {
      if(ch == KEY_DOWN) {
        choose = 2;
        colorSnake = 1;
        colorTetris = 7;
      }   
    }else if(choose == 2) {
      if(ch == KEY_UP) {
        choose = 1;
        colorSnake = 7;
        colorTetris = 1;
      }
    }
  }

  return  choose;
}