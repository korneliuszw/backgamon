//
// Created by wired-mac on 08/12/2023.
//

#ifndef BAKAGAMON_SAVE_H
#define BAKAGAMON_SAVE_H

typedef struct Ctx Ctx;

void saveGame(Ctx *ctx);

void loadGame(Ctx *ctx, char *filename);

void toggleSaveSelectMenu(Ctx *ctx);

#endif //BAKAGAMON_SAVE_H
