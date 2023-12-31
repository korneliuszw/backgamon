//
// Created by wired-mac on 14/11/2023.
//

#ifndef BAKAGAMON_MOVE_H
#define BAKAGAMON_MOVE_H

#define DIRECTION_UP 1
#define DIRECTION_DOWN 0

#include <stdbool.h>

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
    bool band;
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

void selectMove(GameState *gameState, int direction);

void getMoves(GameState *, Board *);

List *listCopy(List *tree);


#endif //BAKAGAMON_MOVE_H
