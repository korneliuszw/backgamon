//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_RENDER_H
#define BAKAGAMON_RENDER_H

#include <Board.h>
#include <State.h>
#include <History.h>
#include <ncurses.h>


#define BORDER_HORIZONTAL_CHARACTER '-'
#define BORDER_VERTICAL_CHARACTER '|'
#define BORDER_CORNER_CHARACTER '+'
#define BORDER_BOTTOM_MARGIN 5
#define BORDER_TOP_MARGIN 2
#define COLOR_PAIR_NORMAL 1
#define COLOR_PAIR_RED_PLAYER 2
#define COLOR_PAIR_WHITE_PLAYER 3
#define COLOR_PAIR_RED_PLAYER_SELECTED 4
#define COLOR_PAIR_WHITE_PLAYER_SELECTED 5
#define COLOR_PAIR_RED_PLAYER_TARGET 6
#define COLOR_PAIR_WHITE_PLAYER_TARGET 7

#define USE_COLOR(cond, color, win) \
    for (bool breaker = true; breaker; (cond) && wattroff(win,color)) \
        for (cond && wattron(win,color); breaker; breaker = false)

typedef struct {
    WINDOW *handle;
    int width;
    int height;
    bool update;
} WindowInfo;

enum CurrentWindow {
    MAIN_WINDOW,
    LEADER_WINDOW
};

typedef struct Context {
    WindowInfo *windowInfo;
    WindowInfo *boardWindowInfo;
    WindowInfo *statusWindowInfo;
    WindowInfo *modalWindow;
    Board *board;
    GameState *gameState;
    enum CurrentWindow currentWindow;
    void *workingMemory;
} Context;


Context *contextInit();

void drawDice(Context *context);

void drawBoard(Context *context);

WindowInfo *createWindow(int width, int height, bool root, int x, int y);


#endif //BAKAGAMON_RENDER_H
