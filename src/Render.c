//
// Created by wired-mac on 12/11/2023.
//

#include <ncurses.h>
#include <stdlib.h>
#include <Board.h>
#include <Render.h>
#include <State.h>

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
    start_color();
    init_pair(COLOR_PAIR_NORMAL, COLOR_BLACK, COLOR_BLUE);
    init_pair(COLOR_PAIR_RED_PLAYER, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_WHITE_PLAYER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_RED_PLAYER_SELECTED, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_PAIR_WHITE_PLAYER_SELECTED, COLOR_YELLOW, COLOR_WHITE);
    init_pair(COLOR_PAIR_RED_PLAYER_TARGET, COLOR_RED, COLOR_GREEN);
    init_pair(COLOR_PAIR_WHITE_PLAYER_TARGET, COLOR_YELLOW, COLOR_GREEN);
    return win;
}

void drawBorders(Context *context) {
    for (int i = 0; i < context->windowInfo->width; i++) {
        mvwaddch(context->windowInfo->handle, BORDER_TOP_MARGIN, i,
                 i == 0 || i == context->windowInfo->width - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
        mvwaddch(context->windowInfo->handle, context->windowInfo->height - BORDER_BOTTOM_MARGIN, i,
                 i == 0 || i == context->windowInfo->width - 1 ? BORDER_CORNER_CHARACTER : BORDER_HORIZONTAL_CHARACTER);
    }
    for (int i = BORDER_TOP_MARGIN + 1; i < context->windowInfo->height - BORDER_BOTTOM_MARGIN; i++) {
        mvwaddch(context->windowInfo->handle, i, 0, BORDER_VERTICAL_CHARACTER);
        mvwaddch(context->windowInfo->handle, i, context->windowInfo->width - 1, BORDER_VERTICAL_CHARACTER);
    }
    context->windowInfo->update = true;
}

void printHelp(Context *context) {
    mvwprintw(context->windowInfo->handle, context->windowInfo->height - 3, 10,
              "P) Play L) Load S) Save Q) Quit B) Previous move N) Next move");
    context->windowInfo->update = true;
}

void printTitle(Context *context) {
    mvwprintw(context->windowInfo->handle, 1, 2, "Backgamon - Korneliusz Wojnicz ");
    context->windowInfo->update = true;
}

void drawStaticElements(Context *context) {
    drawBorders(context);
    printHelp(context);
    printTitle(context);
}

void drawPoint(Context *context, int point, int x, int y) {
    BoardPoint boardPoint = context->board->points[point];
    char *emptyChecker = point % 2 ? "^^^" : ":::";
    int color =
            boardPoint.player == PLAYER_RED ? COLOR_PAIR(COLOR_PAIR_RED_PLAYER) : COLOR_PAIR(COLOR_PAIR_WHITE_PLAYER);
    int step = point > 11 ? 1 : -1;
    int i = 0;
    USE_COLOR(true, color, context->boardWindowInfo->handle) {
            mvwprintw(context->boardWindowInfo->handle, y += (i * step), x, "|=|");
            for (; i < boardPoint.pieces; i++) {
                USE_COLOR(i == boardPoint.pieces - 1 && context->gameState->selectedPiece == point,
                          boardPoint.player == PLAYER_RED ? COLOR_PAIR(COLOR_PAIR_RED_PLAYER_SELECTED)
                                                          : COLOR_PAIR(COLOR_PAIR_WHITE_PLAYER_SELECTED),
                          context->boardWindowInfo->handle) {
                        mvwprintw(context->boardWindowInfo->handle, y + (i * step), x, "|=|");
                    }
            }
        }
    int targetColor = boardPoint.player == PLAYER_RED ? COLOR_PAIR(COLOR_PAIR_RED_PLAYER_TARGET)
                                                      : COLOR_PAIR(COLOR_PAIR_WHITE_PLAYER_TARGET);
    if (context->gameState->availableMoves.movesCount > 0 &&
        GET_MOVE_FROM_SELECTED_PIECE(context->gameState).to == point) {
        wattron(context->boardWindowInfo->handle, targetColor);
    }
    for (; i < 6; i++) {
        mvwprintw(context->boardWindowInfo->handle, y + (i * step), x, emptyChecker);
        wattroff(context->boardWindowInfo->handle, targetColor);
    }
}


void drawBoardPoints(Context *context, int x, int y) {
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
        drawPoint(context, i, x, y);
        x += step;
    }
}

void drawBoard(Context *context) {
    drawBoardPoints(context, context->boardWindowInfo->width - 30, context->boardWindowInfo->height - 5);
    drawDice(context);
    context->boardWindowInfo->update = true;
}


void drawDice(Context *context) {
    wclear(context->statusWindowInfo->handle);
    if (context->gameState->state == ROLLING_DICE || context->gameState->state == PICKING_PLAYER) {
        wprintw(context->statusWindowInfo->handle, "Press Enter to roll ");
    } else if (context->gameState->state == SELECTING_MOVE) {
        wprintw(context->statusWindowInfo->handle, "Press Enter to move");
    }
    if (context->gameState->player != 0) {
        wprintw(context->statusWindowInfo->handle, "Player: %s ",
                context->gameState->player == PLAYER_WHITE ? "White" : "Red");
    }
    if (context->gameState->state > ROLLING_DICE) {
        for (int i = 0; i < context->gameState->dice->rollsCount; i++) {
            USE_COLOR(i == context->gameState->dice->currentRoll, COLOR_PAIR_RED_PLAYER_SELECTED,
                      context->statusWindowInfo->handle) {
                    if (i == context->gameState->dice->currentRoll) waddch(context->statusWindowInfo->handle, 'c');
                    wprintw(context->statusWindowInfo->handle, "%d ",
                            context->gameState->dice->rolls[i]);
                }
        }
    }
    wrefresh(context->statusWindowInfo->handle);
}

WindowInfo *createWindow(int width, int height, bool root, int x, int y) {
    WindowInfo *windowInfo = malloc(sizeof(WindowInfo));
    windowInfo->handle = root ? win_init() : newwin(height, width, y, x);
    windowInfo->width = width;
    windowInfo->height = height;
    return windowInfo;
}


Context *contextInit() {
    Context *context = malloc(sizeof(Context));
    context->windowInfo = createWindow(100, 40, true, 0, 0);
    context->boardWindowInfo = createWindow(
            context->windowInfo->width - 3,
            context->windowInfo->height - BORDER_TOP_MARGIN - BORDER_BOTTOM_MARGIN - 1,
            false, 2, BORDER_TOP_MARGIN + 1);
    context->board = boardInit();
    context->statusWindowInfo = createWindow(context->windowInfo->width - 5, 1, false, 5,
                                             context->windowInfo->height - 4);
    context->gameState = gameStateInit();
    drawStaticElements(context);
    return context;
}
