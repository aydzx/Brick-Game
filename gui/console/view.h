#ifndef TETRISFRONT_H
#define TETRISFRONT_H
#include <ncurses.h>
#include <sys/time.h>
#include <time.h>

#include "../../brick_game/library/defines.h"
#include "../../brick_game/library/objects.h"
typedef struct WindowGame {
  WINDOW* GameWindow;
  WINDOW* GameBorder;
  WINDOW* GameScore;
  WINDOW* NextFigure;
} WindowGame;

void InitWindow();
void InitColor();
void initWindowGame(WindowGame* gameWindow);
void printGameField(WindowGame* gameWindow, GameInfo_t* data, GameState state);
void printMenu(WINDOW* GameWindow);
void printInfo(WINDOW* GameScore, GameInfo_t* data);
void printNextFigure(WINDOW* NextFigure, GameInfo_t* data);
void printField(WINDOW* GameWindow, GameInfo_t* data);
void printGameOver(WINDOW* GameWindow);
void delitWindows(WindowGame* gameWindow);
void printPause(WINDOW* GameScore);
double wtime();
double getCpuTime(double* lastIterMoment, double* currIterMoment);
int chooseGame(WindowGame* gameWindow);

#endif