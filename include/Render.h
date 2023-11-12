//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_RENDER_H
#define BAKAGAMON_RENDER_H


#include <ncurses.h>

typedef struct {
    WINDOW *handle;
    int width;
    int height;
    bool update;
} Context;


Context *contextInit();


#endif //BAKAGAMON_RENDER_H
