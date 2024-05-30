#ifndef TETRISBACK_H
#define TETRISBACK_H

#include "tetris.h"

/// @brief Initialization of the playing field
/// @param GameField
void initGameField(GameInfo_t* GameField);

/// @brief Normalizing shape coordinates when rotated at the edge of the playing
/// field
/// @param curr
void normalaze(Figure* curr);

/// @brief Initialization of the next shape
/// @param figure
void InitCurrFigure(Figure* figure);

/// @brief Spawn of the shape
/// @param data
void spawnShape(GameInfo_t* data);

/// @brief Function changes coordinates depending on the signal
/// @param data
void moving(GameInfo_t* data);

/// @brief Function moves the shape down according to the timer
/// @param data
void shifting(GameInfo_t* data);

/// @brief Attachment check
/// @param data
void attaching(GameInfo_t* data);

/// @brief Action execution function
/// @param data
void action(GameInfo_t* data);

/// @brief Rotation of the shape
/// @param figure
void rotate(Figure* figure);

/// @brief Movement of the shape to the left
/// @param data
void moveLeft(GameInfo_t* data);

/// @brief Movement of the shape to the right
/// @param data
void moveRight(GameInfo_t* data);

/// @brief Movement of the shape down
/// @param data
void moveDown(GameInfo_t* data);

/// @brief Returning the shape one position up
/// @param figure
void moveUp(Figure* figure);

/// @brief Collision check
/// @param figure
/// @param GameField
/// @return true or false
int checkCollision(Figure* figure, GameInfo_t* GameField);

/// @brief Downward collision check
/// @param data
/// @return true or false
int checkCollisionDown(GameInfo_t* data);

/// @brief Game data update
/// @return  Game data
void updateCurrentState(GameInfo_t* data);

/// @brief Placing a shape on the playing field
/// @param figure
/// @param GameField
void placeShapeOnField(Figure figure, GameInfo_t* GameField);

/// @brief Removing a piece from the playing field
/// @param figure
/// @param GameField
void clearCurrShape(Figure figure, GameInfo_t* GameField);

/// @brief Checks if a specific line in the field is completely filled
/// @param data
/// @return true or false
int checkLine(GameInfo_t* data);

/// @brief Removes a line from the field and moves all lines above it down by
/// one
/// @param GameField
/// @param y
void removeLine(GameInfo_t* GameField, int y);

/// @brief Waiting for the start button
/// @param data
void actionStart(GameInfo_t* data);

/// @brief Obtaining game data
/// @return Game data
GameInfo_t* getAllInfo();

/// @brief Copying a shape
/// @param next
/// @param current
void copyShape(Figure* next, Figure* current);

/// @brief Reading data from a file
/// @return max score
int readMaxScore();

/// @brief Recording a record number of points to file
/// @param maxScore
void writeMaxScore(int maxScore);

/// @brief Upper bound check
/// @param data
/// @return true or false
int checkingTopLine(GameInfo_t* data);

/// @brief game over
/// @param data
void gameOver(GameInfo_t* data);

/// @brief game pause
/// @param data
void actionPause(GameInfo_t* data);

#endif
