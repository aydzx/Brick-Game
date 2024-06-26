#include "../../../brick_game/library/model.h"
extern "C" {
#include "../../../brick_game/library/tetrisBack.h"
}
#include <QKeyEvent>
#include <QObject>
namespace s21 {
class ControllerQt {
 public:
  ControllerQt(Model *model);
  ~ControllerQt();
  GameInfo_t *getInfoSnake() { return model_->getInfo(); }
  GameInfo_t *getInfoTetris() { return getAllInfo(); }

  UserAction_t handleInput(QKeyEvent *event);
  UserAction_t handleInputForChoosGame(QKeyEvent *event);
  void updateCurrentState() { model_->updateCurrentState(); }

 private:
  Model *model_;
};
};  // namespace s21
