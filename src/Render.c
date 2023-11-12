//
// Created by wired-mac on 12/11/2023.
//

#include <Render.h>
#include <ncurses.h>
#include <stdlib.h>

#define BORDER_HORIZONTAL_CHARACTER '-'
#define BORDER_VERTICAL_CHARACTER '|'
#define BORDER_CORNER_CHARACTER '+'
#define BORDER_BOTTOM_MARGIN 5
#define BORDER_TOP_MARGIN 2


WINDOW *win_init() {
    WINDOW *win = initscr();
    cbreak();
    noecho();
    keypad(win, TRUE);
    return win;
}

void drawBorders(Context *context) {
    for (int i = 0; i < context->width; i++) {
        mvwaddch(context->handle, BORDER_TOP_MARGIN, i,
                 i == 0 || i == context->width - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
        mvwaddch(context->handle, context->height - BORDER_BOTTOM_MARGIN, i,
                 i == 0 || i == context->width - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
    }
    for (int i = BORDER_TOP_MARGIN + 1; i < context->height - BORDER_BOTTOM_MARGIN; i++) {
        mvwaddch(context->handle, i, 0, BORDER_VERTICAL_CHARACTER);
        mvwaddch(context->handle, i, context->width - 1, BORDER_VERTICAL_CHARACTER);
    }
    context->update = true;
}

void printHelp(Context *context) {
    mvwprintw(context->handle, context->height - 3, 10, "P) Play L) Load S) Save Q) Quit");
    context->update = true;
}

void printTitle(Context *context) {
    mvwprintw(context->handle, 1, 2, "Backgamon - Korneliusz Wojnicz ");
    context->update = true;
}

void drawStaticElements(Context *context) {
    drawBorders(context);
    printHelp(context);
    printTitle(context);
}

Context *contextInit() {
    WINDOW *win = win_init();
    Context *context = malloc(sizeof(Context));
    context->handle = win;
    context->width = 100;
    context->height = 40;
    context->update = false;
    drawStaticElements(context);
    return context;
}
