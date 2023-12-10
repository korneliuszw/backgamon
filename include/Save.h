//
// Created by wired-mac on 08/12/2023.
//

#ifndef BAKAGAMON_SAVE_H
#define BAKAGAMON_SAVE_H

#include <stdio.h>

typedef struct Ctx Ctx;
typedef struct Dice Dice;

void createFolder(char *folder);

void saveGame(Ctx *ctx);

void loadGame(Ctx *ctx, char *filename);

void diceSerializer(FILE *file, Dice *dice);

void diceDeserializer(FILE *file, Dice *dice);

void toggleSaveSelectMenu(Ctx *ctx);

#endif //BAKAGAMON_SAVE_H
