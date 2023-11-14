#ifndef BAKAGAMON_BOARD_H
#define BAKAGAMON_BOARD_H

#include <stdbool.h>

#define BOARD_POINTS 24
#define PLAYERS 2
#define PLAYER_RED 1
#define PLAYER_WHITE 2


typedef struct {
    int pieces;
    int player;
} BoardPoint;


typedef struct Board {
    BoardPoint points[BOARD_POINTS];
    BoardPoint bars[PLAYERS];
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

bool movePieceOut(Board *board, int player, int from);

bool canMovePiece(Board *board, int player, int from, int to);

Board *boardInit();

int winner(Board *board);

#endif //BAKAGAMON_BOARD_H
