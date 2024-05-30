OBJ_DIR = obj
BUILD_DIR = build
SOURCES_CLI = gui/console/main.cc gui/console/controller/controller.cc gui/console/view.cc 
SOURCES_SNAKE = brick_game/snake/model.cc
SOURCES_TETRIS = brick_game/tetris/tetrisBack.c 
HEADERS_SNAKE = brick_game/snake/model.h
CPPFLAGS = -Wall -Wextra -Wall
NCURSESFLAG = -lncurses
LIB_DIR = lib
GCC = g++
FLAGS=-lgtest
OS=$(shell uname -s)
ifeq ($(OS), Linux)
MV = mv desk/viewqt build/BrickGameDesktop && ./build/BrickGameDesktop
FLAGS+=-pthread
else
MV = mv desk/viewqt.app/Contents/MacOS/viewqt build/BrickGameDesktop
endif




install: cli des

uninstall:
	rm -rf build 

dvi:
	doxygen doxygen/Doxyfile
	open doxygen/html/index.html

cli: s21_snake.a s21_tetris.a build_dir
	$(GCC) $(CPPFLAGS)  $(SOURCES_CLI) $(LIB_DIR)/s21_tetris.a $(LIB_DIR)/s21_snake.a $(NCURSESFLAG) -o $(BUILD_DIR)/brick_game_console

run:
	./build/brick_game_console

des: build_dir
	mkdir desk
	cd desk && qmake ../gui/desktop
	cd desk && make 
	$(MV)
	rm -rf desk

test:
	$(GCC) $(CPPFLAGS) --coverage brick_game/snake/model.cc test_back/tests.cc -o backtest $(FLAGS)
	./backtest

gcov_report:
	gcovr --root . --html --html-details report.html
	open report.html

dist:
	mkdir dist
	cp -R brick_game dist
	cp -R gui dist
	cp Makefile dist
	tar cvzf brick_game_dist.tgz dist
	rm -rf dist


clean:
	rm -rf build lib obj  *.gcda *.gcno *.gcov *.gcovm *.gcovr backtest *.html *.css

s21_tetris.a: lib_dir obj_dir
	$(GCC) $(CPPFLAGS) -c $(SOURCES_TETRIS) -o $(OBJ_DIR)/s21_tetris.o
	ar rcs $(LIB_DIR)/s21_tetris.a $(OBJ_DIR)/s21_tetris.o

s21_snake.a: lib_dir obj_dir
	$(GCC) $(CPPFLAGS) -c $(SOURCES_SNAKE) -o $(OBJ_DIR)/s21_snake.o
	ar rcs $(LIB_DIR)/s21_snake.a $(OBJ_DIR)/s21_snake.o


lib_dir:
	mkdir -p $(LIB_DIR)

build_dir:
	mkdir -p $(BUILD_DIR)

obj_dir:
	mkdir -p $(OBJ_DIR)