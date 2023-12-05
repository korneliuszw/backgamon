//
// Created by wired-mac on 28/11/2023.
//

#include <History.h>
#include <Board.h>
#include <Move.h>
#include <State.h>
#include <string.h>

History *initHistory() {
    History *newHistory = (History *) malloc(sizeof(History));
    newHistory->prev = NULL;
    newHistory->next = NULL;
    return newHistory;
}

void initHistoryItem(History *history) {
    history->prevFromPoint = malloc(sizeof(BoardPoint));
    history->prevToPoint = malloc(sizeof(BoardPoint));
    history->dice = malloc(sizeof(Dice));
    history->move = malloc(sizeof(Move));
}

void start_history_entry(History **history, Board *board, Move *move, Dice *dice) {
    // we are already back in time and we are going to override it!
    if ((*history)->next) {
        free((*history)->next);
        (*history)->next = NULL;
    }
    History *newHistory = (History *) malloc(sizeof(History));
    initHistoryItem(newHistory);
    newHistory->prev = *history;
    (*history)->next = newHistory;
    newHistory->next = NULL;
    memcpy(newHistory->dice, dice, sizeof(Dice));
    memcpy(newHistory->prevFromPoint, board->points + move->from, sizeof(BoardPoint));
    memcpy(newHistory->prevToPoint, board->points + move->to, sizeof(BoardPoint));
    memcpy(newHistory->move, move, sizeof(Move));
    *history = newHistory;
}

void commit_history_entry(History *history, Board *board, Move *move) {
    history->newToPoint = malloc(sizeof(BoardPoint));
    history->newFromPoint = malloc(sizeof(BoardPoint));
    memcpy(history->newFromPoint, board->points + move->from, sizeof(Move));
    memcpy(history->newToPoint, board->points + move->to, sizeof(Move));
}


void history_update(History *history, Board *board, GameState *gameState) {
    if (gameState->dice != NULL) free(gameState->dice);
    memcpy(gameState->dice, history->dice, sizeof(Dice));
    gameState->state = SELECTING_MOVE;
    transitionState(gameState, board);
}

void history_back(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->prev == NULL) return;
    *history = historyItem->prev;
    board->points[historyItem->move->from] = *historyItem->prevFromPoint;
    board->points[historyItem->move->to] = *historyItem->prevToPoint;
    history_update(*history, board, gameState);
}

void history_forward(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->next == NULL) return;
    *history = historyItem->next;
    historyItem = *history;
    board->points[historyItem->move->from] = *historyItem->newFromPoint;
    board->points[historyItem->move->to] = *historyItem->newToPoint;
    if (gameState->dice != NULL) free(gameState->dice);
    memcpy(gameState->dice, historyItem->dice, sizeof(Dice));
    history_update(*history, board, gameState);
}
