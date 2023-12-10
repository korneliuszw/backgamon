//
// Created by wired-mac on 14/11/2023.
//


#include <Move.h>
#include <Board.h>
#include <Render.h>
#include <State.h>
#include <string.h>
#include "ModalList.h"


#define MOVE_STEP(player) ((player) == (PR) ? (-1) : (1))


void selectPiece(GameState *gs, int dir) {
    if (gs->curpiece < 0) return;
    int stp = dir == DIRECTION_DOWN ? -1 : 1;
    for (int i = gs->curpiece + stp; i >= 0 && i < BOARD_POINTS; i += stp) {
        if (gs->mvs.avalmvs[i].mvs != NULL && gs->mvs.avalmvs[i].mvs->root != NULL) {
            gs->curpiece = i;
            gs->curmove = gs->mvs.avalmvs[i].mvs->root->data;
            gs->update = true;
            return;
        }
    }
}

#define GET_ROOT(tree) tree == NULL ? NULL : tree->root

void selectMove(GameState *gs, int dir) {
    if (gs->curpiece < 0) return;
    ListNode *next = GET_ROOT(gs->mvs.avalmvs[gs->curpiece].mvs);
    Move *newMove = gs->curmove;
    int dst = 99;
    while (next != NULL) {
        int cto = ((Move *) next->data)->to;
        int cdst = abs(gs->curmove->to - cto);
        if (((dir == DIRECTION_DOWN && gs->curmove->to > cto) ||
             (dir == DIRECTION_UP && gs->curmove->to < cto)) && cdst < dst) {
            newMove = next->data;
            dst = cdst;
        }
        next = next->next;
    }
    gs->curmove = newMove;
    gs->update = true;
}

Move *pushMove(GameState *gs, int from, int to, Roll *roll) {
    Move *move = malloc(sizeof(Move));
    move->to = to;
    move->from = from;
    move->dices = listInit();
    listPush(move->dices, roll);
    if (gs->mvs.avalmvs[from].mvs == NULL) gs->mvs.avalmvs[from].mvs = listInit();
    listPush(gs->mvs.avalmvs[from].mvs, move);
    gs->mvs.avalmvs[from].mpc += 1;
    return move;
}

bool isLastInHome(Board *brd, int player, int piece) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (brd->pts[i].player == player && brd->pts[i].pieces > 0) {
            if ((player == PR && i > piece) || (player == PW && i < piece))
                return false;
        }
    }
    return true;
}

void getMoveForDice(GameState *gs, Board *brd, int from, Roll *roll) {
    if (!roll->enabled) return;
    if (canMovePiece(brd, gs->player, from, from + MOVE_STEP(gs->player) * roll->roll)) {
        pushMove(gs, from, from + MOVE_STEP(gs->player) * roll->roll, roll);
    } else if (areAllPiecesHome(brd, gs->player)) {
        if (gs->player == PR && roll->roll > from && isLastInHome(brd, gs->player, from)) {
            pushMove(gs, from, -1, roll);
        } else if (gs->player == PW && BOARD_POINTS - roll->roll < from && isLastInHome(brd, gs->player, from)) {
            pushMove(gs, from, BOARD_POINTS, roll);
        }
    }
}

void getMovesForMultipleDices(GameState *gs, Board *brd, int rollc, int acc, int from, Roll *roll) {
    if (rollc == gs->dice->rollsCount) return;
    if (!roll->enabled) return getMovesForMultipleDices(gs, brd, rollc + 1, acc, from, roll + 1);
    int curacc = acc + roll->roll;
    if (acc != 0 && canMovePiece(brd, gs->player, from, from + MOVE_STEP(gs->player) * curacc)) {
        getMovesForMultipleDices(gs, brd, rollc + 1, curacc, from, roll);
        Move *move = pushMove(gs, from, from + MOVE_STEP(gs->player) * curacc, roll);
        for (int i = 0; i <= rollc - 1; i++)
            listPush(move->dices, gs->dice->rolls + i);
    } else if (acc == 0) {
        getMovesForMultipleDices(gs, brd, rollc + 1, acc + roll->roll, from, roll + 1);
    }
}

void clrmvs(Moves *mvs) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (mvs->avalmvs[i].mpc == 0) continue;
        ListNode *next = mvs->avalmvs[i].mvs->root;
        while (next != NULL) {
            listDestroy(((Move *) next->data)->dices);
            next = next->next;
        }
        listDestroy(mvs->avalmvs[i].mvs);
        mvs->avalmvs[i].mvs = NULL;
    }
    mvs->mvc = 0;
    free(mvs->avalmvs);
    mvs->avalmvs = NULL;
}

List *filterAttacks(GameState *gs, Board *brd) {
    List *attack = listInit();
    for (int i = 0; i < BOARD_POINTS; i++) {
        ListNode *next = GET_ROOT(gs->mvs.avalmvs[i].mvs);
        while (next != NULL) {
            Move *mv = next->data;
            if (brd->pts[mv->to].player != gs->player && brd->pts[mv->to].player > 0) listPush(attack, next->data);
            next = next->next;
        }
    }
    return attack;
}

void removeAllButBest(GameState *gs, Move *best) {
    Move *bestCp = malloc(sizeof(Move));
    memcpy(bestCp, best, sizeof(Move));
    List *bestDices = listCopy(best->dices);
    clrmvs(&gs->mvs);
    bestCp->dices = bestDices;
    gs->mvs.avalmvs = calloc(sizeof(MovePos), BOARD_POINTS);
    int from = MIN(BOARD_POINTS - 1, MAX(0, bestCp->from));
    gs->mvs.avalmvs[from].mpc = 1;
    gs->mvs.avalmvs[from].mvs = listInit();
    listPush(gs->mvs.avalmvs[from].mvs, bestCp);
    gs->mvs.mvc = 1;
    gs->curmove = bestCp;
}

void forceBestAttack(GameState *gs, Board *brd) {
    List *attack = filterAttacks(gs, brd);
    if (attack->root == NULL) return;
    ListNode *next = attack->root;
    Move *best = next->data;
    while (next != NULL) {
        Move *cur = next->data;
        if (gs->player == PW ? cur->to < best->to : cur->to > best->to) best = cur;
        next = next->next;
    }
    removeAllButBest(gs, best);
}

void applyFilters(GameState *gs, Board *brd) {
    if (areAllPiecesHome(brd, gs->player)) {
        return;
    } else
        forceBestAttack(gs, brd);
}

void getMovesForAllDice(GameState *gs, Board *brd, int from) {
    getMovesForMultipleDices(gs, brd, 0, 0, from, gs->dice->rolls);
    for (int i = 0; i < gs->dice->rollsCount; i++)
        getMoveForDice(gs, brd, from, gs->dice->rolls + i);
}

void calcMoves(GameState *gs, Board *brd) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (gs->player != brd->pts[i].player) continue;
        gs->mvs.avalmvs[i].mpc = 0;
        getMovesForAllDice(gs, brd, i);
        if (gs->mvs.avalmvs[i].mpc) gs->mvs.mvc += 1;
    }
}


void pickDefaultMove(GameState *gs) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (gs->mvs.avalmvs[i].mpc == 0) continue;
        gs->curpiece = i;
        gs->curmove = ((Move *) gs->mvs.avalmvs[i].mvs->root->data);
        gs->update = true;
        return;
    }
}

void moveOutOfBand(GameState *gs, Board *brd) {
    gs->curpiece = gs->player == PR ? BOARD_POINTS - 1 : 0;
    int from = gs->player == PR ? BOARD_POINTS : -1;
    gs->mvs.avalmvs[gs->curpiece].mvs = listInit();
    for (int i = 0; i < gs->dice->rollsCount; i++) {
        Roll *roll = gs->dice->rolls + i;
        if (!roll->enabled) continue;
        int currentMove = roll->roll * MOVE_STEP(gs->player);
        if (canMoveToDestination(brd, gs->player, from + currentMove)) {
            gs->curmove = pushMove(gs, gs->curpiece, from + currentMove, roll);
            gs->curmove->band = true;
            gs->curmove->from = from;
            gs->mvs.mvc = 1;
        }
    }
}

void getMoves(GameState *gameState, Board *board) {
    Moves *mvs = &gameState->mvs;
    if (mvs->avalmvs != NULL) clrmvs(mvs);
    mvs->avalmvs = calloc(sizeof(Moves), BOARD_POINTS);
    gameState->curpiece = -1;
    if (board->bars[gameState->player - 1].pieces > 0) {
        moveOutOfBand(gameState, board);
        applyFilters(gameState, board);
    } else {
        calcMoves(gameState, board);
        applyFilters(gameState, board);
        pickDefaultMove(gameState);
    }
    if (mvs->mvc == 0) {
        gameState->player = gameState->player == PW ? PR : PW;
        gameState->state = DICE;
        transitionState(gameState, board);
    }
}

List *listInit() {
    List *list = calloc(sizeof(List), 1);
    return list;
}

struct ListNode *listNodeInit() {
    struct ListNode *listNode = calloc(sizeof(struct ListNode), 1);
    return listNode;
}

void listPush(List *tree, void *data) {
    if (tree->root == NULL) {
        tree->root = listNodeInit();
        tree->last = tree->root;
    } else {
        tree->last->next = listNodeInit();
        tree->last = tree->last->next;
    }
    tree->last->data = data;
}

void listDestroy(List *tree) {
    ListNode *next = tree->root;
    ListNode *head;
    while (next != NULL) {
        head = next;
        next = next->next;
        free(head);
    }
}

List *listCopy(List *tree) {
    List *copy = listInit();
    ListNode *next = tree->root;
    while (next != NULL) {
        listPush(copy, next->data);
        next = next->next;
    }
    return copy;
}