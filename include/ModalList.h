//
// Created by wired-mac on 08/12/2023.
//

#ifndef BAKAGAMON_MODALLIST_H
#define BAKAGAMON_MODALLIST_H

typedef struct Ctx Ctx;

struct ModalListEntry {
    void *data;
    char *text;
};

struct ModalList {
    int selected;
    int entries;
    struct ModalListEntry *list;

    // return true if the modal list should be closed
    char (*onSelect)(Ctx *ctx, struct ModalListEntry *modalListEntry);
};

void toggleModalList(Ctx *ctx, struct ModalList *modalList);

void moveModalListCursor(Ctx *ctx, int direction);

void selectModalListEntry(Ctx *ctx);

#endif //BAKAGAMON_MODALLIST_H
