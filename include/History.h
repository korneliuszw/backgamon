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
    bool overwrite;
    int player;
    struct History *next;
    struct History *prev;
} History;

History *initHistory();

void start_history_entry(History **history, int player, Board *board, Move *move, Dice *dice);

void commit_history_entry(History *history, Dice *dice, Board *board, Move *move);

void history_back(History **history, Board *board, GameState *gameState);

void history_forward(History **history, Board *board, GameState *gameState);

void save_history(History *history);

#endif //BAKAGAMON_HISTORY_H
