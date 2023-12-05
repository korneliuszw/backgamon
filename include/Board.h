#ifndef BAKAGAMON_BOARD_H
#define BAKAGAMON_BOARD_H

#include <stdbool.h>

#define BOARD_POINTS 24
#define PLAYERS 2
#define PLAYER_RED 1
#define PLAYER_WHITE 2
#define BAR_POINT 36


typedef struct BoardPoint {
    int pieces;
    int player;
} BoardPoint;


typedef struct Board {
    BoardPoint points[BOARD_POINTS];
    BoardPoint bars[PLAYERS];
    int remainingPieces[PLAYERS];
    int winner;
} Board;

int getPlayerBoardStart(int player);

int getPlayerBoardEnd(int player);

bool areAllPiecesHome(Board *board, int player);

// Rolls dices and returns an array of 2 or 4 integers
// the number of integers depends on the result of the roll
// and is returned to moves ptr
// This function assumes rand is already seeded
int *rollDice(int *moves);

bool movePiece(Board *board, int player, int from, int to);

void movePieceOut(Board *board, int player);

bool canMovePiece(Board *board, int player, int from, int to);

bool canMoveToDestination(Board *board, int player, int to);

Board *boardInit();

void winner(Board *board, int player);

#endif //BAKAGAMON_BOARD_H
