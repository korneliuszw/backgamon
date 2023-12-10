//
// Created by wired-mac on 08/12/2023.
//


#include <Save.h>
#include <Render.h>
#include <Board.h>
#include <State.h>
#include <stdio.h>
#include <string.h>
#include <ModalList.h>

#ifdef WINDOWS
#include <windows.h>
char** get_files(char* directory) {
    // not implemented
    return NULL;
}
#else

#include <dirent.h>
#include <sys/stat.h>

char count_files(char *directory) {
    DIR *dir;
    struct dirent *ent;
    int i = 0;
    if ((dir = opendir(directory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".sav") != NULL) {
                i++;
            }
        }
        closedir(dir);
    }
    return i;
}

char **get_files(char *directory) {
    DIR *dir;
    struct dirent *ent;
    char cap = count_files(directory);
    char **files = malloc(sizeof(char *) * (cap + 1));
    int i = 1;
    files[0] = (char *) cap;
    if ((dir = opendir(directory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".save") != NULL) {
                files[i] = malloc(sizeof(char) * 100);
                strcpy(files[i], ent->d_name);
                i++;
            }
        }
        closedir(dir);
    }
    return files;
}

#endif


void boardSerializer(FILE *file, Board *board) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        fprintf(file, "%d %d %d\n", i, board->pts[i].pieces, board->pts[i].player);
    }
    fprintf(file, "BARS ");
    for (int i = 0; i < PLAYERS; i++) {
        fprintf(file, "%d %d ", board->bars[i].pieces, board->bars[i].player);
    }
    fprintf(file, "\nLEFT: %d %d\n", board->rempic[0], board->rempic[1]);
}

char *stateEnumStringifier(enum State state) {
    switch (state) {
        case START:
            return "START";
        case DICE:
            return "DICE";
        case RESTORE:
            return "RESTORE";
        case SELECT:
            return "SELECT";
    }
}

void diceSerializer(FILE *file, Dice *dice) {
    fprintf(file, "DICE: %d\n", dice->rollsCount);
    for (int i = 0; i < dice->rollsCount; i++) {
        fprintf(file, "%d %d %d\n", i, dice->rolls[i].roll, dice->rolls[i].enabled);
    }
}

void stateSerializer(FILE *file, GameState *gameState) {
    fprintf(file, "STATE: %s\n", stateEnumStringifier(gameState->state));
    fprintf(file, "PLAYER: %d\n", gameState->player);
    fprintf(file, "TIMESTAMP: %ld\n", gameState->timestamp);
    diceSerializer(file, gameState->dice);
}

void createFolder(char *folder) {
#ifdef WINDOWS
    CreateDirectory(folder, NULL);
#else
    mkdir(folder, 0777);
#endif
}

void saveGame(Ctx *ctx) {
    char filename[100];
    createFolder("saves");
    sprintf(filename, "saves/%ld.save", ctx->gs->timestamp);
    FILE *file = fopen(filename, "w");
    boardSerializer(file, ctx->b);
    stateSerializer(file, ctx->gs);
}

void boardDeserializer(FILE *file, Board *board) {
    for (int i = 0; i < BOARD_POINTS; i++) {
        fscanf(file, "%d %d %d\n", &i, &board->pts[i].pieces, &board->pts[i].player);
    }
    fscanf(file, "BARS ");
    for (int i = 0; i < PLAYERS; i++) {
        fscanf(file, "%d %d", &board->bars[i].pieces, &board->bars[i].player);
    }
    fscanf(file, "\nLEFT: %d %d\n", &board->rempic[0], &board->rempic[1]);
}

void diceDeserializer(FILE *file, Dice *dice) {
    fscanf(file, "DICE: %d\n", &dice->rollsCount);
    dice->rolls = malloc(sizeof(Roll *) * dice->rollsCount);
    for (int i = 0; i < dice->rollsCount; i++) {
        fscanf(file, "%d %d %d\n", &i, &dice->rolls[i].roll, &dice->rolls[i].enabled);
    }
}

enum State stateEnumDeserializer(FILE *file) {
    char stateString[10];
    fscanf(file, "STATE: %s\n", stateString);
    if (strcmp(stateString, "DICE") == 0) {
        return DICE;
    } else if (strcmp(stateString, "RESTORE") == 0) {
        return RESTORE;
    } else if (strcmp(stateString, "SELECT") == 0) {
        return SELECT;
    }
    return START;
}

void stateDeserializer(FILE *file, GameState *gameState) {
    gameState->state = stateEnumDeserializer(file);
    fscanf(file, "PLAYER: %d\n", &gameState->player);
    fscanf(file, "TIMESTAMP: %ld\n", &gameState->timestamp);
    diceDeserializer(file, gameState->dice);
}

void loadGame(Ctx *ctx, char *filename) {
    char filedir[100];
    sprintf(filedir, "saves/%s", filename);
    FILE *file = fopen(filedir, "r");
    ctx->b = boardInit();
    ctx->gs = gameStateInit();
    boardDeserializer(file, ctx->b);
    stateDeserializer(file, ctx->gs);
    load_history(&ctx->gs->history);
    transitionState(ctx->gs, ctx->b);
}

char onSelect(Ctx *ctx, struct ModalListEntry *entry) {
    loadGame(ctx, entry->text);
    return 1;
}

struct ModalList *saveSelectMenuInit(Ctx *ctx) {
    struct ModalList *modalList = malloc(sizeof(struct ModalList));
    char **files = get_files("saves");
    char cap = (char) files[0];
    modalList->entries = cap;
    modalList->selected = 0;
    modalList->list = malloc(sizeof(struct ModalListEntry) * cap);
    for (int i = 0; i < cap; i++) {
        modalList->list[i].text = files[i + 1];
        modalList->list[i].data = NULL;
    }
    modalList->onSelect = onSelect;
    return modalList;
}

void toggleSaveSelectMenu(Ctx *ctx) {
    struct ModalList *modalList = ctx->tmpmem;
    if (ctx->curwin == MAIN_WINDOW) {
        modalList = ctx->tmpmem = saveSelectMenuInit(ctx);
        ctx->curwin = SAVE_WINDOW;
    }
    toggleModalList(ctx, modalList);
}
