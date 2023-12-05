//
// Created by wired-mac on 13/11/2023.
//
#include <Input.h>
#include <Render.h>
#include <State.h>
#include <ncurses.h>

#include <Move.h>
#include <History.h>

#define KEY_ENTER_KEYBOARD 13

void handleInput(Context *context) {
    int key = 0;
    while ((key = getch()) != ERR) {
        switch (key) {
            case KEY_ENTER_KEYBOARD: {
                transitionState(context->gameState, context->board);
                break;
            }
            case KEY_UP:
            case KEY_DOWN: {
                selectPiece(context->gameState, context->board, key == KEY_UP ? DIRECTION_UP : DIRECTION_DOWN);
                break;
            }
            case 'b': {
                history_back(&context->gameState->history, context->board, context->gameState);
                transitionState(context->gameState, context->board);
                break;
            }
            case 'n': {
                history_forward(&context->gameState->history, context->board, context->gameState);
                transitionState(context->gameState, context->board);
                break;
            }
            case 'q': {
                exit(0);
            }
        }
    }
}
