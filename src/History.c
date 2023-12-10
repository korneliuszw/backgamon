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
#include <unistd.h>

char *fileName() {
    char *path = calloc(sizeof(char), 50);
    sprintf(path, "history/%ld.hist", getctx()->gs->timestamp);
    return path;
}

FILE *openHist(bool append) {
    createFolder("history");
    char *path = fileName();
    FILE *file = fopen(path, !append ? "w+" : "a+");
    if (file == NULL && append) {
        file = fopen(path, "w+");
    }
    free(path);
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
    history->newToPoint = malloc(sizeof(BoardPoint));
    history->diceNext = malloc(sizeof(Dice));
    history->prevBands = malloc(sizeof(BoardPoint) * 2);
    history->nextBands = malloc(sizeof(BoardPoint) * 2);
}

void moveSerializer(FILE *handle, Move *move) {
    fprintf(handle, "MOVE %d %d %d\n", move->from, move->to, move->band);
}

void barsSerializer(FILE *handle, History *history) {
    for (int i = 0; i < PLAYERS; i++)
        fprintf(handle, "%d %d ", history->prevBands[i].pieces, history->prevBands[i].player);
    for (int i = 0; i < PLAYERS; i++)
        fprintf(handle, "%d %d ", history->nextBands[i].pieces, history->nextBands[i].player);
}

void pointSerializer(FILE *handle, History *cur) {
    fprintf(handle, "NEWTO %d %d\n", cur->newToPoint->pieces, cur->newToPoint->player);
    fprintf(handle, "PREVTO %d %d\n", cur->newToPoint->pieces, cur->newToPoint->player);
}

void saveHistory(History *hst) {
    History *cur = hst;
    FILE *historyHandle = openHist(cur->next == NULL);
    while (cur) {
        fprintf(historyHandle, "%d\n", cur->player);
        pointSerializer(historyHandle, cur);
        moveSerializer(historyHandle, cur->move);
        barsSerializer(historyHandle, hst);
        fprintf(historyHandle, "DICEOLD ");
        diceSerializer(historyHandle, cur->dice);
        fprintf(historyHandle, "DICENEW ");
        diceSerializer(historyHandle, cur->diceNext);
        cur = cur->next;
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

void copyHistoryDice(History *new, Dice *dice) {
    memcpy(new->dice, dice, sizeof(Dice));
    new->dice->rolls = malloc(sizeof(Roll) * dice->rollsCount);
    memcpy(new->dice->rolls, dice->rolls, sizeof(Roll) * dice->rollsCount);
}

void startHistoryEntry(History **hst, int player, Board *brd, Move *mv, Dice *dice) {
    // we are already back in time and we are going to override it!
    History *new = (History *) malloc(sizeof(History));
    initHistoryItem(new);
    if ((*hst)->next) {
        free((*hst)->next);
        (*hst)->next = NULL;
        new->overwrite = true;
    }
    new->prev = *hst;
    (*hst)->next = new;
    new->next = NULL;
    new->player = player;
    copyHistoryDice(new, dice);
    memcpy(new->prevToPoint, brd->pts + mv->to, sizeof(BoardPoint));
    memcpy(new->move, mv, sizeof(Move));
    memcpy(new->prevBands, brd->bars, sizeof(BoardPoint) * 2);
    *hst = new;
}

void commitHistoryEntry(History *history, Dice *dice, Board *board, Move *move) {
    memcpy(history->newToPoint, board->pts + move->to, sizeof(BoardPoint));
    memcpy(history->diceNext, dice, sizeof(Dice));
    history->diceNext->rolls = malloc(sizeof(Roll) * dice->rollsCount);
    memcpy(history->diceNext->rolls, dice->rolls, sizeof(Roll) * dice->rollsCount);
    memcpy(history->nextBands, board->bars, sizeof(BoardPoint) * 2);
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

void historyBack(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->prev == NULL) return;
    *history = historyItem->prev;
    if (historyItem->move->band) {
        board->bars[historyItem->player].pieces += 1;
    } else board->pts[historyItem->move->from].pieces += 1;
    memcpy(board->bars, historyItem->prevBands, sizeof(BoardPoint) * 2);
    board->pts[historyItem->move->to] = *historyItem->prevToPoint;
    history_update(historyItem, board, gameState, false);
}

void historyForward(History **history, Board *board, GameState *gameState) {
    History *historyItem = *history;
    if (historyItem->next == NULL) return;
    *history = historyItem->next;
    historyItem = *history;
    if (historyItem->move->band) {
        board->bars[historyItem->player].pieces -= 1;
    } else board->pts[historyItem->move->from].pieces -= 1;
    memcpy(board->bars, historyItem->nextBands, sizeof(BoardPoint) * 2);
    board->pts[historyItem->move->to] = *historyItem->newToPoint;
    history_update(*history, board, gameState, true);
}


void barsDeserializer(FILE *file, History *history) {
    for (int i = 0; i < PLAYERS; i++)
        fscanf(file, "%d %d\n", &history->prevBands[i].pieces, &history->prevBands[i].player);
    for (int i = 0; i < PLAYERS; i++)
        fscanf(file, "%d %d\n", &history->nextBands[i].pieces, &history->nextBands[i].player);
}

void pointDeserializer(FILE *file, History *history) {
    fscanf(file, "NEWTO %d %d\n", &history->prevToPoint->pieces, &history->prevToPoint->player);
    fscanf(file, "PREVTO %d %d\n", &history->newToPoint->pieces, &history->newToPoint->player);
}

void historyDeserializer(FILE *file, History *hst) {
    fscanf(file, "%d\n", &hst->player);
    hst->newToPoint = malloc(sizeof(BoardPoint));
    hst->prevToPoint = malloc(sizeof(BoardPoint));
    pointDeserializer(file, hst);
    fscanf(file, "MOVE %d %d %d\n", &hst->move->from, &hst->move->to, &hst->move->band);
    barsDeserializer(file, hst);
    fscanf(file, "DICEOLD ");
    diceDeserializer(file, hst->dice);
    fscanf(file, "DICENEW ");
    diceDeserializer(file, hst->diceNext);
}

void loadHistory(History **history) {
    char *path = fileName();
    // TODO: Windows support
    if (access(path, F_OK) == -1) {
        return free(path);
    }
    FILE *historyHandle = fopen(path, "r");
    free(path);
    History *current = *history;
    while (!feof(historyHandle)) {
        current->next = malloc(sizeof(History));
        initHistoryItem(current->next);
        historyDeserializer(historyHandle, current->next);
        current->next->prev = current;
        current = current->next;
        *history = current;
    }

}
