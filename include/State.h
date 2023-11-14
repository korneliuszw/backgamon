//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_STATE_H
#define BAKAGAMON_STATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <Move.h>

typedef struct Board Board;

enum State {
    PICKING_PLAYER,
    ROLLING_DICE,
    SELECTING_MOVE,
};

typedef struct GameState {
    enum State state;
    int player;
    int *moves;
    int movesCount;
    int moveNumber;
    Moves availableMoves;
    int selectedMove;
    int selectedPiece;
    bool update;
    int sleep;
} GameState;


void transitionState(GameState *gameState, Board *board);

GameState *gameStateInit();


#endif //BAKAGAMON_STATE_H
