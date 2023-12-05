//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_RENDER_H
#define BAKAGAMON_RENDER_H

#include <Board.h>
#include <State.h>
#include <History.h>
#include <ncurses.h>

typedef struct {
    WINDOW *handle;
    int width;
    int height;
    bool update;
} WindowInfo;

typedef struct Context {
    WindowInfo *windowInfo;
    WindowInfo *boardWindowInfo;
    WindowInfo *statusWindowInfo;
    Board *board;
    GameState *gameState;
} Context;


Context *contextInit();

void drawDice(Context *context);

void drawBoard(Context *context);


#endif //BAKAGAMON_RENDER_H
