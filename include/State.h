//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_STATE_H
#define BAKAGAMON_STATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <Move.h>

#define SELECTED_BAR 0

#define GMFRMPIC(gst) \
    gst->mvs.avalmvs[gst->curpiece == SELECTED_BAR ? 0 : gst->curpiece]

typedef struct Board Board;
typedef struct History History;

enum State {
    START,
    DICE,
    RESTORE,
    SELECT,
};

typedef struct Dice {
    int *rolls;
    int rollsCount;
    int currentRoll;
} Dice;

typedef struct GameState {
    enum State state;
    int player;
    int curpiece;
    bool update;
    int sleep;
    Dice *dice;
    Moves mvs;
    History *history;
} GameState;


void transitionState(GameState *gameState, Board *board);

int getCurrentRoll(GameState *gameState);

GameState *gameStateInit();


#endif //BAKAGAMON_STATE_H
