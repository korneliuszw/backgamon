//
// Created by wired-mac on 14/11/2023.
//

#ifndef BAKAGAMON_MOVE_H
#define BAKAGAMON_MOVE_H

#define DIRECTION_UP 1
#define DIRECTION_DOWN 0

typedef struct GameState GameState;
typedef struct Board Board;

typedef struct Move {
    int from;
    int to;
} Move;

typedef struct {
    int mvc;
    Move *avalmvs;
} Moves;

void selectPiece(GameState *gameState, Board *board, int direction);

void getMoves(GameState *, Board *);


#endif //BAKAGAMON_MOVE_H
