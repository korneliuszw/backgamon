//
// Created by wired-mac on 12/11/2023.
//

#ifndef BAKAGAMON_STATE_H
#define BAKAGAMON_STATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <Move.h>

#define GMFRMPIC(gst) \
    gst->curmove

typedef struct Board Board;
typedef struct History History;

enum State {
    START,
    DICE,
    RESTORE,
    SELECT,
    ASK_FOR_NAME,
    WAIT_FOR_END,
    END
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
typedef struct PlayerInfo PlayerInfo;

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
    PlayerInfo *playerInfo;
} GameState;


void transitionState(GameState *gameState, Board *board);

GameState *gameStateInit();


#endif //BAKAGAMON_STATE_H
