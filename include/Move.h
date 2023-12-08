//
// Created by wired-mac on 14/11/2023.
//

#ifndef BAKAGAMON_MOVE_H
#define BAKAGAMON_MOVE_H

#define DIRECTION_UP 1
#define DIRECTION_DOWN 0

typedef struct GameState GameState;
typedef struct Board Board;
typedef struct Dice Dice;

typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;
typedef struct List {
    struct ListNode *root;
    struct ListNode *last;
} List;


typedef struct Move {
    int to;
    int from;
    List *dices;
} Move;

typedef struct MovePos {
    int mpc;
    // List of Move
    List *mvs;
} MovePos;

typedef struct {
    int mvc;
    MovePos *avalmvs;
} Moves;

void listPush(List *head, void *data);

List *listInit();

void listDestroy(List *head);

void selectPiece(GameState *gs, int dir);

void selectPieceOrMove(GameState *gameState, Board *board, int direction);

void selectMove(GameState *gameState, int direction);

void confSel(GameState *gameState);

void getMoves(GameState *, Board *);


#endif //BAKAGAMON_MOVE_H
