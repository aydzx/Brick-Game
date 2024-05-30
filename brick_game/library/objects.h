#ifndef OBJECTS_H
#define OBJECTS_H

#include "defines.h"

/// @brief Enumeration of shape types

/// @brief Enumeration action key from user
typedef enum {
  NOSIG,
  START,
  PAUSE,
  TERMINATE,
  LEFT,
  RIGHT,
  DOWN,
  UP,
  ACTION
} UserAction_t;

/// @brief Coordinates of the shape
typedef struct {
  int y;
  int x;
} Pixel;

/// @brief Shape data
typedef struct {
  int typefigure;
  Pixel position;
  Pixel pixels[4];
  int color;
} Figure;

/// @brief Enum game status
typedef enum {
  MENU,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAME_OVER,
  EXIT_STATE,
  STOP
} GameState;

/// @brief Data about the playing field, speed score and next figure
typedef struct {
  int field[HEIGHT][WIDTH];
  Figure next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  GameState state;
  Figure figure;
  UserAction_t action;
} GameInfo_t;

#endif