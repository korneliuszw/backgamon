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
    gst->curmove

typedef struct Board Board;
typedef struct History History;

enum State {
    START,
    DICE,
    RESTORE,
    SELECT,
};


typedef struct Roll {
    int roll;
    bool enabled;
} Roll;

typedef struct Dice {
    Roll *rolls;
    int rollsCount;
    int currentRoll;
} Dice;

typedef struct GameState {
    enum State state;
    int player;
    int curpiece;
    Move *curmove;
    bool update;
    int sleep;
    Dice *dice;
    Moves mvs;
    History *history;
    long timestamp;
} GameState;


void transitionState(GameState *gameState, Board *board);

GameState *gameStateInit();


#endif //BAKAGAMON_STATE_H
