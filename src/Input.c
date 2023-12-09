//
// Created by wired-mac on 13/11/2023.
//
#include <Input.h>
#include <Render.h>
#include <State.h>
#include <ncurses.h>
#include <Save.h>
#include <Move.h>
#include <History.h>
#include <Leaderboard.h>
#include <ModalList.h>

#define KEY_ENTER_KEYBOARD 13

void handleLeaderboardInput(int key, Ctx *Ctx) {
    switch (key) {
        case KEY_UP:
        case KEY_DOWN: {
            return moveModalListCursor(Ctx, key == KEY_UP ? DIRECTION_UP : DIRECTION_DOWN);
        }
        case KEY_ENTER_KEYBOARD: {
            return selectModalListEntry(Ctx);
        }
        case 'q':
        case 't': {
            return toggleModalList(Ctx, Ctx->tmpmem);
        }
    }
}

void handleMainWindowInput(int key, Ctx *Ctx) {
    switch (key) {
        case KEY_ENTER_KEYBOARD: {
            return transitionState(Ctx->gs, Ctx->b);
        }
        case KEY_UP:
        case KEY_DOWN: {
            return selectPiece(Ctx->gs, key == KEY_UP ? DIRECTION_UP : DIRECTION_DOWN);
        }
        case KEY_LEFT:
        case KEY_RIGHT: {
            return selectMove(Ctx->gs, key == KEY_LEFT ? DIRECTION_UP : DIRECTION_DOWN);
        }
        case 'b': {
            history_back(&Ctx->gs->history, Ctx->b, Ctx->gs);
            return transitionState(Ctx->gs, Ctx->b);
        }
        case 'n': {
            history_forward(&Ctx->gs->history, Ctx->b, Ctx->gs);
            return transitionState(Ctx->gs, Ctx->b);
        }
        case 't': {
            return toggleLeaderboard(Ctx);
        }
        case 'l': {
            return toggleSaveSelectMenu(Ctx);
        }
        case 's': {
            return saveGame(Ctx);
        }
        case 'q': {
            exit(0);
        }
    }
}

void handleInput(Ctx *Ctx) {
    int key = 0;
    while ((key = getch()) != ERR) {
        if (Ctx->curwin == MAIN_WINDOW) {
            handleMainWindowInput(key, Ctx);
        } else {
            handleLeaderboardInput(key, Ctx);
        }
    }
}
