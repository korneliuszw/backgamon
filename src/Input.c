//
// Created by wired-mac on 13/11/2023.
//
#include <Input.h>
#include <Render.h>
#include <State.h>
#include <ncurses.h>

#include <Move.h>

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
            case 'q': {
                exit(0);
            }
        }
    }
}
