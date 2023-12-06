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

void handleLeaderboardInput(int key, Context *context) {
    switch (key) {
        case KEY_UP:
        case KEY_DOWN: {
            return moveLeaderboardCursor(context, key == KEY_UP ? -1 : 1);
        }
        case KEY_ENTER_KEYBOARD:
        case 'q':
        case 't': {
            return toggleLeaderboard(context);
        }
    }
}

void handleMainWindowInput(int key, Context *context) {
    switch (key) {
        case KEY_ENTER_KEYBOARD: {
            return transitionState(context->gameState, context->board);
        }
        case KEY_UP:
        case KEY_DOWN: {
            return selectPiece(context->gameState, context->board, key == KEY_UP ? DIRECTION_UP : DIRECTION_DOWN);
        }
        case 'b': {
            history_back(&context->gameState->history, context->board, context->gameState);
            return transitionState(context->gameState, context->board);
        }
        case 'n': {
            history_forward(&context->gameState->history, context->board, context->gameState);
            return transitionState(context->gameState, context->board);
        }
        case 't': {
            return toggleLeaderboard(context);
        }
        case 'q': {
            exit(0);
        }
    }
}

void handleInput(Context *context) {
    int key = 0;
    while ((key = getch()) != ERR) {
        if (context->currentWindow == MAIN_WINDOW) {
            handleMainWindowInput(key, context);
        } else if (context->currentWindow == LEADER_WINDOW) {
            handleLeaderboardInput(key, context);
        }
    }
}
