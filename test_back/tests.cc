#include <gtest/gtest.h>
#include "../brick_game/snake/model.h"
#include <iostream>

TEST(FSM, Menu) {
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();

  ASSERT_EQ(info->speed, 1);
  ASSERT_EQ(info->score, 0);
  ASSERT_EQ(info->level, 1);
  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 1);
}

TEST(FSM, Spawn){ 
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();

  info->state = SPAWN;
  model.updateCurrentState();

  //первоначальное положение змейки на поле
  for(int i = 0; i < 3; i++){
    ASSERT_EQ(info->field[3][1+i], 2);
  }
  int  count_food = 0;
  for(int i = 0; i < HEIGHT; i++){ // положение яблока на поле
    for(int j = 0; j < WIDTH; j++){
      if(info->field[i][j] == 1){
        count_food++;
      }
    }
  }

  // for(int i = 0; i < 3; i++){ // проверка на то, что при спавне яблоко не попало в змейку
  //   ASSERT_NE(1+i, food_j);
  // }
  ASSERT_EQ(count_food, 1); // яблоко одно
  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 3);
}

TEST(FSM, MOVING){ // яблоко может попасть в старое положение змейки и провалить второй тест
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();

  info->state = MOVING;
  info->action = DOWN;
  model.updateCurrentState();

  ASSERT_EQ(info->field[3][2], 2); //проверка на движение вниз
  ASSERT_EQ(info->field[3][3], 2);
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[4][4], 2);

  info->state = MOVING;
  info->action = RIGHT;
  model.updateCurrentState();

  ASSERT_EQ(info->field[3][3], 2); //проверка на движение вправа
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[4][4], 2);
  ASSERT_EQ(info->field[4][5], 2);

  info->state = MOVING;
  info->action = UP;
  model.updateCurrentState();

  ASSERT_EQ(info->field[3][4], 2); //проверка на движение вверх
  ASSERT_EQ(info->field[3][5], 2);
  ASSERT_EQ(info->field[4][4], 2);
  ASSERT_EQ(info->field[4][5], 2);

  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 3);
}

TEST(FSM, MOVING_LEFT){ // яблоко может попасть в старое положение змейки и провалить второй тест
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();

  info->state = SHIFTING;
  info->action = DOWN;
  model.updateCurrentState();

  ASSERT_EQ(info->field[3][2], 2); //проверка на движение вниз
  ASSERT_EQ(info->field[3][3], 2);
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[4][4], 2);

  info->action = LEFT;
  model.updateCurrentState();

  ASSERT_EQ(info->field[3][3], 2); //проверка на движение влева
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[4][3], 2);
  ASSERT_EQ(info->field[4][4], 2);

  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 3);
}
TEST(FSM, MOVING_OPPOSITE_SIDE){ // яблоко может попасть в старое положение змейки и провалить второй тест
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();
  info->state = SHIFTING;
  info->action = DOWN;
  model.updateCurrentState();
  info->state = SHIFTING;
  info->action = UP;
  for(auto i = 0 ; i < 12; ++i){
  model.updateCurrentState();
  }

  ASSERT_EQ(info->field[3][3], 2); //проверка на движение вниз
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[4][4], 2);
  ASSERT_EQ(info->field[5][4], 2);


  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 3);
}

TEST(FSM, SHIFTING){
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();
  info->state = SPAWN;
  model.updateCurrentState();
  info->state = SHIFTING;
  for(auto i = 0 ; i < 12; ++i){
  model.updateCurrentState();
  }

  ASSERT_EQ(info->field[3][2], 2); //проверка на движение вниз
  ASSERT_EQ(info->field[3][3], 2);
  ASSERT_EQ(info->field[3][4], 2);
  ASSERT_EQ(info->field[3][5], 2);

  ASSERT_EQ(info->action, 0);
  //ASSERT_EQ(info->state, 3);
}
TEST(FSM, PAUSE){
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();
  info->state = SPAWN;
  model.updateCurrentState();
  info->state = SHIFTING;
  info->action = PAUSE;
  for(auto i = 0 ; i < 12; ++i){
  model.updateCurrentState();
  }
  

  ASSERT_EQ(info->field[3][1], 2); //проверка на движение вниз
  ASSERT_EQ(info->field[3][2], 2);
  ASSERT_EQ(info->field[3][3], 2);
  ASSERT_EQ(info->field[3][4], 2);

  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 7);
}
TEST(FSM, GAME_OVER){
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();
  info->state = SPAWN;
  model.updateCurrentState();
  info->state = SHIFTING;
  info->action = UP;
  model.updateCurrentState();
  model.updateCurrentState();
  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 5);
  info->action = TERMINATE;
  model.updateCurrentState();
  ASSERT_EQ(info->action, 0);
  ASSERT_EQ(info->state, 6);
}
TEST(FSM, EAT_APPLE){
  s21::Model model;
  GameInfo_t *info;
  info = model.getInfo();
  info->state = MENU;
  info->action = START;
  model.updateCurrentState();
  info->state = SPAWN;
  model.updateCurrentState();
  info->state = SHIFTING;
  
  // 0 1 2 3 4 5 6 7 8 9  3
  // 0 1 2 3 4 5 6 7 8 9  4
  // 0 1 2 3 4 5 6 7 8 9  5
  // 0 1 2 3 4 5 6 7 8 9  6
  // 0 1 2 3 4 5 6 7 8 9  7
  // 0 1 2 3 4 5 6 7 8 9  8
  // 0 1 2 3 4 5 6 7 8 9  9
  // 0 1 2 3 4 5 6 7 8 9 10 
  // 0 1 2 3 4 5 6 7 8 9 11 
  // 0 1 2 3 4 5 6 7 8 9 12 
  // 0 1 2 3 4 5 6 7 8 9 13
  // 0 1 2 3 4 5 6 7 8 9 14
  // 0 1 2 3 4 5 6 7 8 9 15
  // 0 1 2 3 4 5 6 7 8 9 16
  // 0 1 2 3 4 5 6 7 8 9 17
  // 0 1 2 3 4 5 6 7 8 9 18
  // 0 1 2 3 4 5 6 7 8 9 19
  // 0 1 2 3 4 5 6 7 8 9 20
  // 0 1 2 3 4 5 6 7 8 9 21
  // 0 1 2 3 4 5 6 7 8 9 22
   for(auto i = 0 ; i < 5; ++i){
    info->state = SHIFTING;
    info->action = RIGHT;
  model.updateCurrentState();
  }
  info->state = SHIFTING;
     info->action = DOWN;
  model.updateCurrentState();



for(auto i = 0 ; i < 9; ++i){
       info->state = SHIFTING;
     
  for(auto i = 0 ; i < 9; ++i){
  info->state = SHIFTING;
    info->action = LEFT;
  model.updateCurrentState();
  }
info->state = SHIFTING;
     info->action = DOWN;
  model.updateCurrentState();
     for(auto i = 0 ; i < 9; ++i){
    info->state = SHIFTING;
    info->action = RIGHT;
  model.updateCurrentState();
  }
info->state = SHIFTING;
       info->action = DOWN;
  model.updateCurrentState();
     }
      for(auto i = 0 ; i < 9; ++i){
  info->state = SHIFTING;
    info->action = LEFT;
  model.updateCurrentState();
  }
        for(auto i = 0 ; i < 19; ++i){
  info->state = SHIFTING;
    info->action = UP;
  model.updateCurrentState();
  }
// for(auto i = 3 ; i < 23; ++i){
//   for(auto j = 0 ; j < 10; ++j){
//     std::cout << info->field[i][j];
//   }
//   std::cout << std::endl;
// }
  ASSERT_EQ(info->field[3][0], 2); 
  ASSERT_EQ(info->field[4][0], 2);
  ASSERT_EQ(info->field[5][0], 2);
  ASSERT_EQ(info->field[6][0], 2);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}