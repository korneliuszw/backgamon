//
// Created by wired-mac on 28/11/2023.
//

#ifndef BAKAGAMON_HISTORY_H
#define BAKAGAMON_HISTORY_H

#include <stdlib.h>
#include <stdio.h>

typedef struct BoardPoint BoardPoint;
typedef struct Move Move;
typedef struct Board Board;
typedef struct Dice Dice;
typedef struct GameState GameState;

#include <stdbool.h>

typedef struct History {
    Dice *dice;
    Dice *diceNext;
    Move *move;
    BoardPoint *newToPoint;
    BoardPoint *prevToPoint;
    BoardPoint *prevBands;
    BoardPoint *nextBands;
    bool overwrite;
    int player;
    struct History *next;
    struct History *prev;
} History;

History *initHistory();

void startHistoryEntry(History **hst, int player, Board *brd, Move *mv, Dice *dice);

void commitHistoryEntry(History *history, Dice *dice, Board *board, Move *move);

void historyBack(History **history, Board *board, GameState *gameState);

void historyForward(History **history, Board *board, GameState *gameState);

void loadHistory(History **history);

#endif //BAKAGAMON_HISTORY_H
