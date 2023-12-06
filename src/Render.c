//
// Created by wired-mac on 12/11/2023.
//

#include <ncurses.h>
#include <stdlib.h>
#include <Board.h>
#include <Render.h>
#include <State.h>


void pair_init() {
    start_color();
    init_pair(CPN, COLOR_BLACK, COLOR_BLUE);
    init_pair(CPRP, COLOR_RED, COLOR_BLACK);
    init_pair(CPWP, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CPRPS, COLOR_RED, COLOR_WHITE);
    init_pair(CPWPS, COLOR_YELLOW, COLOR_WHITE);
    init_pair(CPRPT, COLOR_RED, COLOR_GREEN);
    init_pair(CPWPT, COLOR_YELLOW, COLOR_GREEN);
}

WINDOW *win_init() {
    WINDOW *win = initscr();
    cbreak();
    noecho();
    nodelay(win, TRUE);
    keypad(win, TRUE);
    raw();
    nonl();
    if (has_colors() == false) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    pair_init();
    return win;
}

// draw borders of a border
void drwbor(Ctx *Ctx) {
    for (int i = 0; i < Ctx->wminf->w; i++) {
        mvwaddch(Ctx->wminf->handle, BORDER_TOP_MARGIN, i,
                 i == 0 || i == Ctx->wminf->w - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
        mvwaddch(Ctx->wminf->handle, Ctx->wminf->h - BORDER_BOTTOM_MARGIN, i,
                 i == 0 || i == Ctx->wminf->w - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
    }
    for (int i = BORDER_TOP_MARGIN + 1; i < Ctx->wminf->h - BORDER_BOTTOM_MARGIN; i++) {
        mvwaddch(Ctx->wminf->handle, i, 0, BORDER_VERTICAL_CHARACTER);
        mvwaddch(Ctx->wminf->handle, i, Ctx->wminf->w - 1, BORDER_VERTICAL_CHARACTER);
    }
    Ctx->wminf->update = true;
}

// draw menu help
void drwhelp(Ctx *Ctx) {
    mvwprintw(Ctx->wminf->handle, Ctx->wminf->h - 3, 10,
              "P) Play L) Load S) Save Q) Quit B) Previous move N) Next move T) Hall of Fame");
    Ctx->wminf->update = true;
}

// draw game name
void drwname(Ctx *Ctx) {
    mvwprintw(Ctx->wminf->handle, 1, 2, "Backgamon - Korneliusz Wojnicz ");
    Ctx->wminf->update = true;
}

// draw static elements for menu
void drwmenu(Ctx *Ctx) {
    drwbor(Ctx);
    drwhelp(Ctx);
    drwname(Ctx);
}

// draw p pieces
int drawppic(Ctx *ctx, BoardPoint *bp, int p, int stp, int *y, int x) {
    int i = 0;
    USE_COLOR(true, bp->player == PR ? COLOR_PAIR(CPRP) : COLOR_PAIR(CPWP), ctx->wbinf->handle) {
            mvwprintw(ctx->wbinf->handle, *y += (i * stp), x, "|=|");
            for (; i < bp->pieces; i++) {
                USE_COLOR(i == bp->pieces - 1 && ctx->gs->curpiece == p,
                          bp->player == PR ? COLOR_PAIR(CPRPS)
                                           : COLOR_PAIR(CPWPS),
                          ctx->wbinf->handle) {
                        mvwprintw(ctx->wbinf->handle, *y + (i * stp), x, "|=|");
                    }
            }
        }
    return i;
}

// draw board pt from given pt
void drwp(Ctx *ctx, int pt, int x, int y) {
    BoardPoint bp = ctx->b->pts[pt];
    char *empty = pt % 2 ? "^^^" : ":::";
    int stp = pt > 11 ? 1 : -1;
    int i = drawppic(ctx, &bp, pt, stp, &y, x);
    int tgc = bp.player == PR ? COLOR_PAIR(CPRPT)
                              : COLOR_PAIR(CPWPT);
    ctx->gs->mvs.mvc > 0 && GMFRMPIC(ctx->gs).to == pt && wattron(ctx->wbinf->handle, tgc);
    for (; i < 6; i++) {
        mvwprintw(ctx->wbinf->handle, y + (i * stp), x, empty);
        wattroff(ctx->wbinf->handle, tgc);
    }
}

// draw board pts starting from  x and y coordinate
void drwbp(Ctx *Ctx, int x, int y) {
    int step = -5;
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (i == 12) {
            y = 5;
            x = 5;
            step = 5;
        }
        if (i == 6) {
            x -= 6;
        } else if (i == 18) {
            x += 6;
        }
        drwp(Ctx, i, x, y);
        x += step;
    }
}

// draw whole board
void drwb(Ctx *Ctx) {
    drwbp(Ctx, Ctx->wbinf->w - 30, Ctx->wbinf->h - 5);
    drwd(Ctx);
    Ctx->wbinf->update = true;
}


// draw move status
void drwms(Ctx *ctx) {
    if (ctx->gs->state == DICE || ctx->gs->state == START) {
        wprintw(ctx->wsinf->handle, "Press Enter to roll ");
    } else if (ctx->gs->state == SELECT) {
        wprintw(ctx->wsinf->handle, "Press Enter to move");
    }
}

// draw current player move
void drawp(Ctx *ctx) {
    if (ctx->gs->player != 0) {
        wprintw(ctx->wsinf->handle, "Player: %s ",
                ctx->gs->player == PW ? "White" : "Red");
    }
}

// draw value of a single dice
void drwdval(Ctx *ctx, int it) {
    if (it == ctx->gs->dice->currentRoll) waddch(ctx->wsinf->handle, 'c');
    wprintw(ctx->wsinf->handle, "%d ",
            ctx->gs->dice->rolls[it]);
}

// draw move and dice status
void drwd(Ctx *ctx) {
    wclear(ctx->wsinf->handle);
    drwms(ctx);
    drawp(ctx);
    if (ctx->gs->state > DICE) {
        for (int i = 0; i < ctx->gs->dice->rollsCount; i++) {
            USE_COLOR(i == ctx->gs->dice->currentRoll, CPRPS,
                      ctx->wsinf->handle) {
                    drwdval(ctx, i);
                }
        }
    }
    wrefresh(ctx->wsinf->handle);
}

// create window with given parameters
WindowInfo *crwin(int width, int height, bool root, int x, int y) {
    WindowInfo *windowInfo = malloc(sizeof(WindowInfo));
    windowInfo->handle = root ? win_init() : newwin(height, width, y, x);
    windowInfo->w = width;
    windowInfo->h = height;
    return windowInfo;
}


void initWindows(Ctx *ctx) {
    ctx->wminf = crwin(100, 40, true, 0, 0);
    ctx->wbinf = crwin(
            ctx->wminf->w - 3,
            ctx->wminf->h - BORDER_TOP_MARGIN - BORDER_BOTTOM_MARGIN - 1,
            false, 2, BORDER_TOP_MARGIN + 1);
    ctx->wsinf = crwin(ctx->wminf->w - 5, 1, false, 5,
                       ctx->wminf->h - 4);
}

Ctx *getctx() {
    static Ctx *ctx = NULL;
    if (ctx == NULL) {
        ctx = malloc(sizeof(Ctx));
    }
    return ctx;
}

Ctx *CtxInit() {
    Ctx *ctx = getctx();
    ctx->curwin = MAIN_WINDOW;
    initWindows(ctx);
    ctx->b = boardInit();
    ctx->gs = gameStateInit();
    drwmenu(ctx);
    return ctx;
}
