QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    viewqt.cpp \
    controller/controllerQt.cc \
    ../../brick_game/snake/model.cc \
    ../../brick_game/tetris/tetrisBack.c
    

HEADERS += \
    viewqt.h \
    controller/controllerQt.h \
    ../../brick_game/snake/model.h \
    ../../brick_game/tetris/library/tetrisBack.h

FORMS += \
    viewqt.ui

LIBS += -lncurses



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
