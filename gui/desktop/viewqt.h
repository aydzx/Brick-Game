#ifndef VIEWQT_H
#define VIEWQT_H
#include <QTimer>
#include <QWidget>
#include <QPainter>
#include <QObject>
#include "controller/controllerQt.h"
#include "../../brick_game/snake/model.h"
extern "C" {
#include "../../brick_game/tetris/library/tetrisBack.h"
}


QT_BEGIN_NAMESPACE
namespace Ui {
class ViewQt;
}
QT_END_NAMESPACE

namespace s21{
class ViewQt : public QWidget
{
Q_OBJECT

public:
    void printInfo();
    void handleTimer();
    void drawGame(QPainter* painter);
    ViewQt(s21::ControllerQt *controller, QWidget *parent = nullptr);
    ~ViewQt();
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *);
private:
    std::vector<Qt::GlobalColor> color_ = {Qt::white,Qt::red,Qt::green,Qt::blue,Qt::yellow, Qt::cyan, Qt::darkBlue,Qt::darkGray};
    GameInfo_t* data;
    UserAction_t action;
    int flagGame;
    Qt::GlobalColor tetrisColor;
    Qt::GlobalColor snakeColor;
    void choosGame(QPainter* painter);
    bool game_selected;
    Ui::ViewQt *ui;
    s21::ControllerQt *controller_;
    QTimer *timer;
};
};
#endif // VIEWQT_H
