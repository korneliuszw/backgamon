#ifndef BAKAGAMON_BOARD_H
#define BAKAGAMON_BOARD_H

#include <stdbool.h>

#define BOARD_POINTS 24
#define PLAYERS 2
// red player index
#define PR 1
// white player index
#define PW 2
#define BAR_POINT 36

typedef struct Roll Roll;
typedef struct Move Move;


typedef struct BoardPoint {
    int pieces;
    int player;
} BoardPoint;


typedef struct Board {
    BoardPoint pts[BOARD_POINTS];
    BoardPoint bars[PLAYERS];
    int rempic[PLAYERS];
    int winner;
} Board;

int getPlayerBoardStart(int player);

int getPlayerBoardEnd(int player);

bool areAllPiecesHome(Board *board, int player);

// Rolls dices and returns an array of 2 or 4 integers
// the number of integers depends on the result of the roll
// and is returned to mvs ptr
// This function assumes rand is already seeded
Roll *rollDice(int *moves);

bool movePiece(Board *board, int player, Move *mv);

void movePieceOut(Board *board, int player, Move *mv);

bool canMovePiece(Board *board, int player, int from, int to);

bool canMoveToDestination(Board *board, int player, int to);

Board *boardInit();

int calculateWinPoints(Board *board);

#endif //BAKAGAMON_BOARD_H
