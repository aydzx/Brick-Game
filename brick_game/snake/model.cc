#include "../library/model.h"

void s21::Model::updateCurrentState() {
  preliminaryDataCheck();

  if (game_info_.state == MENU) {
    initGame();
  } else if (game_info_.state == SPAWN) {
    spawnFood();
  } else if (game_info_.state == MOVING) {
    moving();
  } else if (game_info_.state == SHIFTING) {
    shifting();
  } else if (game_info_.state == STOP) {
    statePause();
  } else if (game_info_.state == GAME_OVER) {
    gameOver();
  }
  game_info_.action = NOSIG;
}

void s21::Model::preliminaryDataCheck() {
  if ((game_info_.action == UP || game_info_.action == DOWN ||
       game_info_.action == RIGHT || game_info_.action == LEFT) &&
      game_info_.state == SHIFTING) {
    game_info_.state = MOVING;
    if (!checkPrevAction()) {  // проверка на предыдущее действие (движение в
                               // противоположную сторону)
      game_info_.action = prev_action_;
      game_info_.state = SHIFTING;
    }
  }
  if (game_info_.action == NOSIG && game_info_.state == SHIFTING) {
    game_info_.action = prev_action_;
  }
}
void s21::Model::initGame() {
  if (game_info_.action == START) {
    game_info_.speed = 1;
    game_info_.score = 0;
    game_info_.level = 1;
    game_info_.action = RIGHT;
    prev_action_ = RIGHT;
    time_for_shifting_ = 0;
    srand(time(0));
    initData();
    game_info_.state = SPAWN;
  }
}
void s21::Model::initData() {
  placeholderForNext();  // заглушка для next
  initSnake();
  readFromFile();
  clearField();
  placeSnakeToField();
}
void s21::Model::initSnake() {
  snake.resize(SNAKE_START_LENGTH);
  for (std::vector<std::pair<int, int>>::size_type i = 0; i < snake.size();
       i++) {
    snake[i].first = snake.size() - i;
    snake[i].second = 0 + OFFSET_Y;
  }

  len_snake_ = SNAKE_START_LENGTH;
}
void s21::Model::initFood() {
  // Генерируем случайную позицию для яблока
  food = std::make_pair(rand() % MAX_WIDTH, rand() % MAX_HEIGHT + OFFSET_Y);
}

void s21::Model::spawnFood() {
  clearFoodFromField();
  generateFoodWithoutCollision();
  placeSnakeToField();
  placeFoodToField();
  game_info_.state = SHIFTING;
}

void s21::Model::moving() {
  clearSnakeFromField();
  updateData();
  placeSnakeToField();
}
void s21::Model::updateData() {
  std::pair<int, int> newPix = snake[0];

  // Обновляем координаты головы змейки в зависимости от направления
  if (game_info_.action == UP) {
    newPix.second--;
    prev_action_ = UP;

  } else if (game_info_.action == DOWN) {
    newPix.second++;
    prev_action_ = DOWN;

  } else if (game_info_.action == RIGHT) {
    newPix.first++;
    prev_action_ = RIGHT;

  } else if (game_info_.action == LEFT) {
    newPix.first--;
    prev_action_ = LEFT;
  }
  if (checkingCollision(newPix)) {
    movementSnake(newPix);
  }
}

void s21::Model::shifting() {
  if (game_info_.action == TERMINATE) {
    writeToFile();
    game_info_.state = GameState::EXIT_STATE;
  } else if (game_info_.action == PAUSE) {
    game_info_.state = STOP;
  } else {
    if (time_for_shifting_ > 11 - game_info_.speed) {
      clearField();
      updateData();
      placeSnakeToField();
      placeFoodToField();
      time_for_shifting_ = 0;
    }
    ++time_for_shifting_;
  }
}
bool s21::Model::checkingCollision(std::pair<int, int>& newPix) {
  // Проверка на выход за границы окна
  if (newPix.first < 0 || newPix.first >= WIDTH || newPix.second < OFFSET_Y ||
      newPix.second >= HEIGHT - 1) {
    game_info_.state = GameState::GAME_OVER;
    return false;
  }
  // Проверка на столкновение с собственным телом
  if (std::find(snake.begin(), snake.end(), newPix) != snake.end()) {
    game_info_.state = GameState::GAME_OVER;
    return false;
  }

  return true;
}

void s21::Model::movementSnake(std::pair<int, int>& newPix) {
  if (newPix.first == food.first &&
      newPix.second == food.second) {  // проверка на столкновение с яблоком
    // Добавляем новую голову змейки в начало списка координат
    snake.insert(snake.begin(), newPix);
    len_snake_++;
    game_info_.score += 1;
    if (game_info_.score > game_info_.high_score) {
      game_info_.high_score = game_info_.score;
    }

    game_info_.level = game_info_.score / 5 + 1;
    if (game_info_.level == MAXLEVEL) {
      game_info_.level = MAXLEVEL;
    }
    game_info_.speed = game_info_.level;
    if (game_info_.score == MAX_SCORE_SNAKE) {
      game_info_.state = GAME_OVER;
    } else {
      game_info_.state = SPAWN;
    }
  } else {
    // Добавляем новую голову змейки в начало списка координат
    snake.insert(snake.begin(), newPix);
    // Удаляем последний сегмент хвоста змейки
    snake.pop_back();
    time_for_shifting_ = 0;
    game_info_.state = SHIFTING;
  }
}

void s21::Model::generateFoodWithoutCollision() {
  // // Пока яблоко не попадёт в змейку, генерируем новое
  do {
    initFood();
  } while (std::find(snake.begin(), snake.end(), food) != snake.end());
}

void s21::Model::placeSnakeToField() {
  for (std::vector<std::pair<int, int>>::size_type i = 0; i < snake.size();
       i++) {
    game_info_.field[snake[i].second][snake[i].first] = 2;
  }
}
void s21::Model::placeFoodToField() {
  game_info_.field[food.second][food.first] = 1;
}

void s21::Model::clearField() {
  for (auto i = 0; i < HEIGHT; i++) {
    for (auto j = 0; j < WIDTH; j++) {
      game_info_.field[i][j] = 0;
    }
  }
}

bool s21::Model::checkPrevAction() {
  if (prev_action_ == UP && game_info_.action == DOWN) {
    return false;
  } else if (prev_action_ == LEFT && game_info_.action == RIGHT) {
    return false;
  } else if (prev_action_ == RIGHT && game_info_.action == LEFT) {
    return false;
  } else if (prev_action_ == DOWN && game_info_.action == UP) {
    return false;
  }
  return true;
}

void s21::Model::writeToFile() {
  std::ofstream fout(std::filesystem::path("HighScoreFile.txt"));
  if (fout.is_open()) {
    fout << game_info_.high_score << std::endl;
    fout.close();
  }
}

void s21::Model::readFromFile() {
  std::ifstream infile(std::filesystem::path("HighScoreFile.txt"));
  std::string score;
  if (infile.is_open()) {
    if (infile >> score) {
      game_info_.high_score = std::stoi(score);
    }
    infile.close();
  } else {
    game_info_.high_score = 0;
  }
}

void s21::Model::clearFoodFromField() {
  game_info_.field[food.second][food.first] = 0;
}
void s21::Model::clearSnakeFromField() {
  for (std::vector<std::pair<int, int>>::size_type i = 0; i < snake.size();
       i++) {
    game_info_.field[snake[i].second][snake[i].first] = 0;
  }
}

void s21::Model::statePause() {
  if (game_info_.action == PAUSE) {
    game_info_.state = SHIFTING;
  }
}

void s21::Model::gameOver() {
  if (game_info_.action == TERMINATE) {
    writeToFile();
    game_info_.state = GameState::EXIT_STATE;
  } else if (game_info_.action == START) {
    initGame();
    game_info_.state = SPAWN;
  }
}
s21::Model::Model() { game_info_.state = MENU; }

s21::Model::~Model() { writeToFile(); }

void s21::Model::placeholderForNext() {
  for (int i = 0; i < 4; i++) {
    game_info_.next.pixels[i].x = 0;
    game_info_.next.pixels[i].y = 0;
  }
}
