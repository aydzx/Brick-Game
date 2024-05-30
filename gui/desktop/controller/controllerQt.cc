#include "controllerQt.h"

#include <QDebug>

UserAction_t s21::ControllerQt::handleInput(QKeyEvent *event) {
  auto input = event->key();

  if (input == Qt::Key_Up) {
    return UP;
  } else if (input == Qt::Key_Down) {
    return DOWN;
  } else if (input == Qt::Key_Left) {
    return LEFT;
  } else if (input == Qt::Key_Right) {
    return RIGHT;
  } else if (input == Qt::Key_Space) {
    return ACTION;
  } else if (input == Qt::Key_Return) {
    return START;
  } else if (input == Qt::Key_P) {
    return PAUSE;
  } else if (input == Qt::Key_Escape) {
    return TERMINATE;
  }

  return NOSIG;
}
UserAction_t s21::ControllerQt::handleInputForChoosGame(QKeyEvent *event) {
  auto input = event->key();
  if (input == Qt::Key_Up) {
    return UP;
  } else if (input == Qt::Key_Down) {
    return DOWN;
  } else if (input == Qt::Key_Return) {
    return START;
  }
  return NOSIG;
}

s21::ControllerQt::ControllerQt(s21::Model *model) : model_(model){};
s21::ControllerQt::~ControllerQt(){};
