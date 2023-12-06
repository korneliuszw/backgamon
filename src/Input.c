//
// Created by wired-mac on 13/11/2023.
//
#include <Input.h>
#include <Render.h>
#include <State.h>
#include <ncurses.h>

#include <Move.h>
#include <History.h>
#include <Leaderboard.h>

#define KEY_ENTER_KEYBOARD 13

void handleLeaderboardInput(int key, Ctx *Ctx) {
    switch (key) {
        case KEY_UP:
        case KEY_DOWN: {
            return moveLeaderboardCursor(Ctx, key == KEY_UP ? -1 : 1);
        }
        case KEY_ENTER_KEYBOARD:
        case 'q':
        case 't': {
            return toggleLeaderboard(Ctx);
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
            return selectPiece(Ctx->gs, Ctx->b, key == KEY_UP ? DIRECTION_UP : DIRECTION_DOWN);
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
        } else if (Ctx->curwin == LEADER_WINDOW) {
            handleLeaderboardInput(key, Ctx);
        }
    }
}
