#include "viewqt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    s21::Model model;
    s21::ControllerQt controller(&model);
    s21::ViewQt w(&controller);
    w.setFixedSize(491, 602);
    w.show();
    return a.exec();
}
