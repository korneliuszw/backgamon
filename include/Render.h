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
#define CPN 1
// color pair for red player
#define CPRP 2
// color pair for white player
#define CPWP 3
// color pair for red player selected
#define CPRPS 4
// color pair for white player selected
#define CPWPS 5
// color pair for red player target
#define CPRPT 6
// color pair for white player target
#define CPWPT 7

// color pair for red player potential target
#define CPWPPT 8
// color pair for white player potential target
#define CPRPPT 9

#define USE_COLOR(c, col, win) \
    for (bool b=true;b;(c)&&wattroff(win,col)) \
        for (c&&wattron(win,col);b;b=false)

typedef struct {
    WINDOW *handle;
    int w;
    int h;
    bool update;
} WindowInfo;

enum CurWin {
    MAIN_WINDOW,
    LEADER_WINDOW
};

typedef struct Ctx {
    WindowInfo *wminf;
    WindowInfo *wbinf;
    WindowInfo *wsinf;
    WindowInfo *wmodinf;
    Board *b;
    GameState *gs;
    enum CurWin curwin;
    void *tmpmem;
} Ctx;


Ctx *CtxInit();

Ctx *getctx();

void drwd(Ctx *ctx);

void drwb(Ctx *Ctx);

WindowInfo *crwin(int w, int h, bool root, int x, int y);


#endif //BAKAGAMON_RENDER_H
