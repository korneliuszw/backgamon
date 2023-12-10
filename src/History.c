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
    history->prevToPoint = malloc(sizeof(BoardPoint));
    history->dice = malloc(sizeof(Dice));
    history->move = malloc(sizeof(Move));
}

void start_history_entry(History **history, int player, Board *board, Move *move, Dice *dice) {
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
    newHistory->player = player;
    memcpy(newHistory->dice, dice, sizeof(Dice));
    memcpy(newHistory->prevToPoint, board->pts + move->to, sizeof(BoardPoint));
    memcpy(newHistory->move, move, sizeof(Move));
    *history = newHistory;
}

void commit_history_entry(History *history, Board *board, Move *move) {
    history->newToPoint = malloc(sizeof(BoardPoint));
    memcpy(history->newToPoint, board->pts + move->to, sizeof(BoardPoint));
}


void history_update(History *history, Board *board, GameState *gameState) {
    if (history->dice)
        memcpy(gameState->dice, history->dice, sizeof(Dice));
    gameState->player = history->player;
    gameState->state = RESTORE;
}

void history_back(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->prev == NULL) return;
    *history = historyItem->prev;
    if (historyItem->move->band) {
        board->bars[historyItem->player].pieces += 1;
    } else board->pts[historyItem->move->from].pieces += 1;
    board->pts[historyItem->move->to] = *historyItem->prevToPoint;
    history_update(historyItem, board, gameState);
}

void history_forward(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->next == NULL) return;
    *history = historyItem->next;
    historyItem = *history;
    if (historyItem->move->band) {
        board->bars[historyItem->player].pieces -= 1;
    } else board->pts[historyItem->move->from].pieces -= 1;
    board->pts[historyItem->move->to] = *historyItem->newToPoint;
    if (gameState->dice != NULL) free(gameState->dice);
    memcpy(gameState->dice, historyItem->dice, sizeof(Dice));
    history_update(*history, board, gameState);
}
