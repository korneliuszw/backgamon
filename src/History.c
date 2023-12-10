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
    newHistory->dice->rolls = malloc(sizeof(Roll) * dice->rollsCount);
    memcpy(newHistory->dice->rolls, dice->rolls, sizeof(Roll) * dice->rollsCount);
    memcpy(newHistory->prevToPoint, board->pts + move->to, sizeof(BoardPoint));
    memcpy(newHistory->move, move, sizeof(Move));
    *history = newHistory;
}

void commit_history_entry(History *history, Dice *dice, Board *board, Move *move) {
    history->newToPoint = malloc(sizeof(BoardPoint));
    memcpy(history->newToPoint, board->pts + move->to, sizeof(BoardPoint));
    history->diceNext = malloc(sizeof(Dice));
    memcpy(history->diceNext, dice, sizeof(Dice));
    history->diceNext->rolls = malloc(sizeof(Roll) * dice->rollsCount);
    memcpy(history->diceNext->rolls, dice->rolls, sizeof(Roll) * dice->rollsCount);
}


void history_update(History *history, Board *board, GameState *gameState, bool forward) {
    if (history->dice) {
        Dice *dice = forward ? history->diceNext : history->dice;
        if (gameState->dice->rolls) free(gameState->dice->rolls);
        memcpy(gameState->dice, dice, sizeof(Dice));
        gameState->dice->rollsCount = dice->rollsCount;
        gameState->dice->rolls = malloc(sizeof(Roll) * dice->rollsCount);
        memcpy(gameState->dice->rolls, dice->rolls, sizeof(Roll) * dice->rollsCount);
    }
    gameState->player = history->player;
    gameState->state = RESTORE;
    transitionState(gameState, board);
}

void history_back(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->prev == NULL) return;
    *history = historyItem->prev;
    if (historyItem->move->band) {
        board->bars[historyItem->player].pieces += 1;
    } else board->pts[historyItem->move->from].pieces += 1;
    board->pts[historyItem->move->to] = *historyItem->prevToPoint;
    history_update(historyItem, board, gameState, false);
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
    history_update(*history, board, gameState, true);
}
