
#include "controller/controller.h"
#include "../../brick_game/snake/model.h"
#include "../../brick_game/tetris/library/tetris.h"
#include "view.h"

int main() {
InitWindow();
  WindowGame gameWindow;
  initWindowGame(&gameWindow);
  box(gameWindow.GameBorder, 0, 0);
  double lastIterMoment = wtime();
  double currIterMoment;
  double threshold = 1.0 / MAXFPS;
  int numberGame = 0;
  s21::Model *model;
  GameInfo_t *info;
while(info->state != EXIT_STATE) {
 numberGame = chooseGame(&gameWindow);
  if(numberGame == 2){
    model = new s21::Model;
    info = model->getInfo();
    info->state = GameState::MENU;
    info->action = UserAction_t::RIGHT;
  }else{
    info = getAllInfo();
    info->state = GameState::MENU;
  }
  while (info->state != EXIT_STATE) {
    double CpuTime = getCpuTime(&lastIterMoment, &currIterMoment);
        
      handleInput(info);
    if (CpuTime > threshold) {
       
      lastIterMoment = currIterMoment;
      if(numberGame == 2){
        model->updateCurrentState(); 
      } else {
        updateCurrentState(info);
        
      }
      printGameField(&gameWindow, info, info->state);
    }
  }
  if(numberGame == 1){
  writeMaxScore(info->score);
}
}


  delitWindows(&gameWindow);

    return 0;
}