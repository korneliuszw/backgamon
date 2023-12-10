//
// Created by wired-mac on 28/11/2023.
//

#include <History.h>
#include <Board.h>
#include <Move.h>
#include <State.h>
#include <Render.h>
#include <string.h>
#include <Save.h>

FILE *openHistoryFile(bool append) {
    createFolder("history");
    char path[50];
    sprintf(path, "history/%ld.hist", getctx()->gs->timestamp);
    FILE *file = fopen(path, !append ? "w+" : "a+");
    if (file == NULL && append) {
        file = fopen(path, "w+");
    }
    return file;
}

History *initHistory() {
    History *newHistory = (History *) malloc(sizeof(History));
    newHistory->prev = NULL;
    newHistory->next = NULL;
    return newHistory;
}

void initHistoryItem(History *history) {
    history->overwrite = false;
    history->prevToPoint = malloc(sizeof(BoardPoint));
    history->dice = malloc(sizeof(Dice));
    history->move = malloc(sizeof(Move));
}

void moveSerializer(FILE *handle, Move *move) {
    fprintf(handle, "MOVE %d %d %d\n", move->from, move->to, move->band);
}

void saveHistory(History *history) {
    History *current = history;
    FILE *historyHandle = openHistoryFile(current->next == NULL);
    while (current) {
        fprintf(historyHandle, "%d\n", current->player);
        fprintf(historyHandle, "NEWTO %d %d\n", current->newToPoint->pieces, current->newToPoint->player);
        fprintf(historyHandle, "PREVTO %d %d\n", current->newToPoint->pieces, current->newToPoint->player);
        moveSerializer(historyHandle, current->move);
        fprintf(historyHandle, "DICEOLD ");
        diceSerializer(historyHandle, current->dice);
        fprintf(historyHandle, "DICENEW ");
        diceSerializer(historyHandle, current->diceNext);
        current = current->next;
    }
    fclose(historyHandle);
}

void saveOrOverwriteHistory(History *history) {
    if (history->overwrite) {
        while (history->prev && history->prev->dice) {
            history = history->prev;
        }
    }
    saveHistory(history);
}

void start_history_entry(History **history, int player, Board *board, Move *move, Dice *dice) {
    // we are already back in time and we are going to override it!
    History *newHistory = (History *) malloc(sizeof(History));
    initHistoryItem(newHistory);
    if ((*history)->next) {
        free((*history)->next);
        (*history)->next = NULL;
        newHistory->overwrite = true;
    }
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
    saveOrOverwriteHistory(history);
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
