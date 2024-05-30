#include "viewqt.h"

#include "ui_viewqt.h"

s21::ViewQt::ViewQt(s21::ControllerQt *controller, QWidget *parent)
    : QWidget(parent), ui(new Ui::ViewQt), controller_(controller) {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &s21::ViewQt::handleTimer);
  timer->start(50);
  setFocusPolicy(Qt::StrongFocus);
  ui->setupUi(this);
  game_selected = false;
  flagGame = 0;
  tetrisColor = Qt::red;
  snakeColor = Qt::green;
  action = NOSIG;
}

s21::ViewQt::~ViewQt() { delete ui; }

void s21::ViewQt::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  ui->GameField->update();
  if (!game_selected) {
    choosGame(&painter);
  } else {
    if (data->state == MENU) {
      ui->Menu->setText("     Press Enter");
    } else if (data->state == STOP) {
      ui->Menu->setText("     P A U S E");
    } else if (data->state == GAME_OVER) {
      if (data->score == 200 && flagGame == 1) {
        ui->Menu->setText("    YOU WIN !\npress enter to restart");
      } else {
        ui->Menu->setText("     GAME OVER\npress enter to restart");
      }
    } else {
      ui->Menu->setText("");
      drawGame(&painter);
    }
  }
}

void s21::ViewQt::handleTimer() {
  if (game_selected) {
    if (flagGame == 0) {
      updateCurrentState(data);
    } else {
      controller_->updateCurrentState();
    }
    if (data->state == EXIT_STATE) {
      close();
    }
    ui->GameField->update();
  }
}
void s21::ViewQt::drawGame(QPainter *painter) {
  for (int y = 3; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (data->field[y][x]) {
        painter->setBrush(color_[data->field[y][x]]);
        painter->drawRect(x * PIXEL, (y - OFFSET_Y) * PIXEL, 29, 29);
      }
    }
  }
  if (flagGame == 0) {
    for (int i = 0; i < 4; i++) {
      painter->setBrush(color_[data->next.color]);
      painter->drawRect((data->next.pixels[i].x * PIXEL) + 250,
                        (data->next.pixels[i].y + 1) * PIXEL, 29, 29);
    }
  }
  painter->end();
  printInfo();
}

void s21::ViewQt::keyPressEvent(QKeyEvent *event) {
  if (game_selected) {
    data->action = controller_->handleInput(event);
    ui->GameField->update();
  } else {
    action = controller_->handleInputForChoosGame(event);
    ui->GameField->update();
  }
}

void s21::ViewQt::choosGame(QPainter *painter) {
  QFont font("Arial");  // Set the font family and size
  font.setPixelSize(14);
  painter->setFont(font);
  //    painter->s
  painter->drawText(2 * PIXEL, 1 * PIXEL, "CHOOSE A GAME");
  QPen pen(tetrisColor);
  painter->setPen(pen);

  QBrush brush(Qt::NoBrush);  // Установите цвет фона в "нет"
  painter->setBrush(brush);

  pen.setColor(tetrisColor);
  painter->drawText(3 * PIXEL, 10 * PIXEL,
                    "TETRIS");  // Отрисуйте текст с помощью цвета шрифта

  pen.setColor(snakeColor);  // Установите цвет шрифта для следующего текста
  painter->setPen(pen);

  painter->drawText(3 * PIXEL, 12 * PIXEL, "SNAKE");

  if (flagGame == 0 && action == DOWN) {
    flagGame = 1;
    tetrisColor = Qt::green;
    snakeColor = Qt::red;
    ui->GameField->update();
  } else if (flagGame == 1 && action == UP) {
    flagGame = 0;
    tetrisColor = Qt::red;
    snakeColor = Qt::green;
    ui->GameField->update();
  } else if (action == START) {
    if (flagGame == 0) {
      game_selected = true;
      data = controller_->getInfoTetris();
      ui->GameField->update();
    } else {
      game_selected = true;
      data = controller_->getInfoSnake();
      ui->GameField->update();
    }
    data->state = MENU;
  }
}

void s21::ViewQt::printInfo() {
  ui->lable_value_lv->setText(QString::number(data->level));
  ui->lable_value_score->setText(QString::number(data->score));
  ui->lable_value_speed->setText(QString::number(data->speed));
  ui->lable_value_HiScore->setText(QString::number(data->high_score));
}
