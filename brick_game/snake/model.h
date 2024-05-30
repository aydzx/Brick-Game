#ifndef MODEL_H
#define MODEL_H
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../tetris/library/defines.h"
#include "../tetris/library/objects.h"

namespace s21 {

/// @brief A model of the MVC paternoster in which snake and "apple"
/// initialization is implemented. This class implements checks for collisions,
/// coordinate changes when changing the direction of movement
class Model {
 public:
  Model();
  ~Model();
  void updateCurrentState();
  GameInfo_t *getInfo() { return &game_info_; }

 private:
  void initGame();
  void initData();
  void initSnake();
  void initFood();
  void moving();
  void shifting();
  void statePause();
  void gameOver();
  void spawnFood();
  void generateFoodWithoutCollision();
  void preliminaryDataCheck();
  void updateData();
  bool checkPrevAction();
  void movementSnake(std::pair<int, int> &newPix);
  bool checkingCollision(std::pair<int, int> &newPix);
  void readFromFile();
  void writeToFile();
  void clearField();
  void clearFoodFromField();
  void clearSnakeFromField();
  void placeSnakeToField();
  void placeFoodToField();
  GameInfo_t game_info_;
  int len_snake_;
  std::vector<std::pair<int, int>> snake;
  std::pair<int, int> food;
  UserAction_t prev_action_;
  int time_for_shifting_;
  void placeholderForNext();
};
};  // namespace s21
#endif
