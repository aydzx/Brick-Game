#include "library/tetris.h"

void updateCurrentState(GameInfo_t* data) {
  if (data->state == SHIFTING && data->state != STOP) {
    if (data->action == ACTION || data->action == DOWN ||
        data->action == LEFT || data->action == RIGHT) {
      data->state = MOVING;
    }
  }
  if (data->action == TERMINATE) {
    data->state = EXIT_STATE;
  }

  if (data->state == MENU) {
    actionStart(data);
  } else if (data->state == SPAWN) {
    spawnShape(data);
  } else if (data->state == MOVING) {
    moving(data);
  } else if (data->state == SHIFTING) {
    shifting(data);
  } else if (data->state == ATTACHING) {
    attaching(data);
  } else if (data->state == GAME_OVER) {
    gameOver(data);
  } else if (data->state == STOP) {
    actionPause(data);
  }

  data->action = NOSIG;
}

void initGameField(GameInfo_t* GameField) {
  memset(GameField->field, 0, sizeof(GameField->field));
  GameField->score = 0;
  GameField->level = 1;
  GameField->speed = 1;
  GameField->high_score = readMaxScore();
  InitCurrFigure(&GameField->next);
}

void InitCurrFigure(Figure* figure) {
  const short figures[7][4][2] = {
      {{0, 1}, {1, 1}, {2, 1}, {3, 1}},  // I
      {{1, 1}, {2, 1}, {1, 2}, {2, 2}},  // O
      {{1, 1}, {2, 1}, {3, 1}, {1, 2}},  // L
      {{0, 1}, {1, 1}, {2, 1}, {2, 2}},  // J
      {{1, 0}, {1, 1}, {2, 1}, {1, 2}},  // T
      {{1, 1}, {2, 1}, {0, 2}, {1, 2}},  // S
      {{1, 1}, {2, 1}, {2, 2}, {3, 2}},  // Z
  };
  figure->position.y = 0;
  figure->position.x = 3;

  // int type = (TypeFigure)(rand() % NUMBER_OF_FIGURES);
  figure->typefigure = (rand() % NUMBER_OF_FIGURES);
  figure->color = 1 + rand() % NUMBER_OF_FIGURES;

  for (int pixel = 0; pixel < 4; pixel++) {
    figure->pixels[pixel].x =
        figures[figure->typefigure][pixel][0] + figure->position.x;
    figure->pixels[pixel].y = figures[figure->typefigure][pixel][1];
  }
}

void normalaze(Figure* figure) {
  int minX = WIDTH, minY = HEIGHT;
  int maxX = 0, maxY = 0;

  // Находим минимальные и максимальные координаты фигуры
  for (int i = 0; i < 4; i++) {
    minX = (figure->pixels[i].x < minX) ? figure->pixels[i].x : minX;
    minY = (figure->pixels[i].y < minY) ? figure->pixels[i].y : minY;
    maxX = (figure->pixels[i].x > maxX) ? figure->pixels[i].x : maxX;
    maxY = (figure->pixels[i].y > maxY) ? figure->pixels[i].y : maxY;
  }

  int offsetX = 0, offsetY = 0;
  if (minX < 0) {
    offsetX = -minX;
  } else if (maxX >= WIDTH) {
    offsetX = WIDTH - 1 - maxX;
  }
  if (minY < 0) {
    offsetY = -minY;
  } else if (maxY >= HEIGHT) {
    offsetY = HEIGHT - 1 - maxY;
  }

  // Смещаем все пиксели фигуры на offsetX и offsetY
  figure->position.x += offsetX;
  figure->position.y += offsetY;
  for (int i = 0; i < 4; i++) {
    figure->pixels[i].x += offsetX;
    figure->pixels[i].y += offsetY;
  }
}

void rotate(Figure* figure) {
  for (int i = 0; i < 4; i++) {
    int temp = figure->pixels[i].x - figure->position.x;
    figure->pixels[i].x = 3 - figure->pixels[i].y + figure->position.y;
    figure->pixels[i].x += figure->position.x;
    figure->pixels[i].y = temp + figure->position.y;
  }
}

void moveLeft(GameInfo_t* data) {
  clearCurrShape(data->figure, data);
  data->figure.position.x--;
  for (int x = 0; x < 4; x++) {
    data->figure.pixels[x].x--;
  }
  if (checkCollision(&data->figure, data)) {
    data->figure.position.x++;
    for (int x = 0; x < 4; x++) {
      data->figure.pixels[x].x++;
    }
  }
  placeShapeOnField(data->figure, data);
}

void moveRight(GameInfo_t* data) {
  clearCurrShape(data->figure, data);
  data->figure.position.x++;
  for (int x = 0; x < 4; x++) {
    data->figure.pixels[x].x++;
  }
  if (checkCollision(&data->figure, data)) {
    data->figure.position.x--;
    for (int x = 0; x < 4; x++) {
      data->figure.pixels[x].x--;
    }
  }
  placeShapeOnField(data->figure, data);
}

void moveDown(GameInfo_t* data) {
  clearCurrShape(data->figure, data);
  data->figure.position.y++;
  for (int y = 0; y < 4; y++) {
    data->figure.pixels[y].y++;
  }
  if (checkCollisionDown(data)) moveUp(&data->figure);
  placeShapeOnField(data->figure, data);
}

void moveUp(Figure* figure) {
  figure->position.y--;
  for (int y = 0; y < 4; y++) {
    figure->pixels[y].y--;
  }
}

int checkCollision(Figure* figure, GameInfo_t* GameField) {
  int check = 0;  // Нет столкновения
  for (int i = 0; i < 4; i++) {
    int x = figure->pixels[i].x;
    int y = figure->pixels[i].y;

    // Проверяем, не выходит ли фигура за границы игрового поля по вертикали
    if (y >= HEIGHT || (y >= 0 && GameField->field[y][x] != 0)) {
      check = 1;  // Столкновение с нижней границей поля
    }

    // Проверяем, не выходит ли фигура за границы игрового поля по горизонтали
    if (x < 0 || x >= WIDTH) {
      check = 1;  // Столкновение с боковыми границами поля
    }
  }

  return check;
}
int checkCollisionDown(GameInfo_t* data) {
  int check = 0;  // Нет столкновения
  for (int i = 0; i < 4; i++) {
    int x = data->figure.pixels[i].x;
    int y = data->figure.pixels[i].y;

    // Проверяем, не выходит ли фигура за границы игрового поля по вертикали
    if (y >= 23 || (y >= 0 && data->field[y][x] != 0)) {
      check = 1;  // Столкновение с нижней границей поля
      data->state = ATTACHING;
    }
  }

  return check;
}

void placeShapeOnField(Figure figure, GameInfo_t* GameField) {
  for (int i = 0; i < 4; i++) {
    GameField->field[figure.pixels[i].y][figure.pixels[i].x] = figure.color;
  }
}

void clearCurrShape(Figure figure, GameInfo_t* GameField) {
  for (int i = 0; i < 4; i++) {
    GameField->field[figure.pixels[i].y][figure.pixels[i].x] = 0;
  }
}

int checkLine(GameInfo_t* data) {
  int x = 0;
  int Score = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH && data->field[y][x]; x++);
    if (x == 10) {
      removeLine(data, y);
      Score = Score * 2 + 100;
    }
  }
  return Score;
}

void removeLine(GameInfo_t* GameField, int y) {
  for (int h = y; h > 2; h--) {
    for (int x = 0; x < 10; x++) {
      GameField->field[h][x] = GameField->field[h - 1][x];
    }
  }
}

GameInfo_t* getAllInfo() {
  static GameInfo_t all_info;
  return &all_info;
}
void spawnShape(GameInfo_t* data) {
  copyShape(&data->next, &data->figure);
  if (checkCollision(&data->figure, data)) {
    data->state = GAME_OVER;
  } else {
    InitCurrFigure(&data->next);
    placeShapeOnField(data->figure, data);
    data->state = SHIFTING;
  }
}

void actionStart(GameInfo_t* data) {
  if (data->action == START) {
    initGameField(data);
    data->state = SPAWN;
  }
}

void shifting(GameInfo_t* data) {
  static int time_for_shifting_ = 0;
  if (time_for_shifting_ >
      11 - data->speed) {  // изменение скорости меняет вызов moveDown на 50
                           // миллисекунд
    moveDown(data);
    time_for_shifting_ = 0;
  }
  if (data->action == PAUSE) {
    data->state = STOP;
  }

  time_for_shifting_++;
}

void attaching(GameInfo_t* data) {
  int levelThreshold = 600;  // уровень меняется каждые 600 очков
  data->score += checkLine(data);
  if (data->score > data->high_score) {
    data->high_score = data->score;
  }
  data->level = data->score / levelThreshold +
                1;  // +1 , чтобы уровень не начинался с нуля
  if (data->level > MAXLEVEL) {
    data->level = MAXLEVEL;
  }
  data->speed = data->level;
  if (checkingTopLine(data)) {
    data->state = GAME_OVER;
  } else {
    data->state = SPAWN;
  }
}

void action(GameInfo_t* data) {
  clearCurrShape(data->figure, data);
  Figure temp;
  copyShape(&data->figure, &temp);
  rotate(&temp);
  normalaze(&temp);
  if ((checkCollision(&temp, data)) == 0) {
    rotate(&data->figure);
    normalaze(&data->figure);
    placeShapeOnField(data->figure, data);
  } else {
    placeShapeOnField(data->figure, data);
  }
}

void moving(GameInfo_t* data) {
  if (data->action == ACTION) {
    action(data);
  } else if (data->action == DOWN) {
    moveDown(data);
  } else if (data->action == LEFT) {
    moveLeft(data);
  } else if (data->action == RIGHT) {
    moveRight(data);
  }
  data->state = SHIFTING;
  shifting(data);
}

void copyShape(Figure* next, Figure* current) {
  for (int i = 0; i < 4; i++) {
    current->pixels[i].x = next->pixels[i].x;
    current->pixels[i].y = next->pixels[i].y;
  }
  current->position = next->position;
  current->color = next->color;
  current->typefigure = next->typefigure;
}

int readMaxScore() {
  int maxScore = 0;
  FILE* file = fopen("MaxScore.txt", "r");
  if (file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      maxScore = maxScore * 10 + (ch - '0');
    }
    fclose(file);
  }
  return maxScore;
}

void writeMaxScore(int maxScore) {
  FILE* file = fopen("MaxScore.txt", "w");
  if (file) {
    fprintf(file, "%d", maxScore);
    fclose(file);
  }
}

int checkingTopLine(GameInfo_t* data) {
  int check = 0;
  for (int i = 0; i < WIDTH && !check; i++) {
    if (data->field[2][i]) {
      check = 1;
    }
  }
  return check;
}

void gameOver(GameInfo_t* data) {
  writeMaxScore(data->high_score);
  actionStart(data);
}

void actionPause(GameInfo_t* data) {
  if (data->action == PAUSE) {
    data->state = SHIFTING;
  }
}
