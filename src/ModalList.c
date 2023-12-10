
//
// Created by wired-mac on 08/12/2023.
//
#include <ModalList.h>
#include <Render.h>
#include <ncurses.h>
#include <string.h>

void drawModalList(Ctx *ctx, struct ModalList *modalList) {
    werase(ctx->wmodinf->handle);
    box(ctx->wmodinf->handle, 0, 0);
    int start = modalList->selected - (modalList->selected % (ctx->wmodinf->h - 2));
    int y = 0;
    for (int i = 0; i < ctx->wmodinf->h - 2; i++) {
        int idx = start + i;
        if (idx >= modalList->entries) break;
        if (idx == modalList->selected)
            y = i + 1;
        mvwprintw(ctx->wmodinf->handle, i + 1, 1, modalList->list[idx].text);
    }
    if (modalList->selected < modalList->entries)
        wmove(ctx->wmodinf->handle, y, strlen(modalList->list[modalList->selected].text) + 1);
    wrefresh(ctx->wmodinf->handle);
}

void freeModalList(Ctx *ctx, struct ModalList *modalList) {
    wclear(ctx->wmodinf->handle);
    wrefresh(ctx->wmodinf->handle);
    delwin(ctx->wmodinf->handle);
    ctx->wmodinf->handle = NULL;
    for (int i = 0; i < modalList->entries; i++) {
        free(modalList->list[i].text);
        if (modalList->list[i].data != NULL) free(modalList->list[i].data);
    }
    free(modalList->list);
    free(ctx->tmpmem);
    ctx->tmpmem = NULL;
    free(ctx->wmodinf);
    ctx->wmodinf = NULL;
}

void toggleModalList(Ctx *ctx, struct ModalList *modalList) {
    if (ctx->wmodinf == NULL) {
        ctx->wmodinf = crwin(50, 10, false, ctx->wminf->w / 2 - 5,
                             ctx->wminf->h / 2 - 5);
        drawModalList(ctx, modalList);
        ctx->tmpmem = modalList;
    } else {
        freeModalList(ctx, modalList);
        ctx->curwin = MAIN_WINDOW;
    }
}

void moveModalListCursor(Ctx *ctx, int direction) {
    struct ModalList *modalList = (struct ModalList *) ctx->tmpmem;
    modalList->selected += direction == DIRECTION_DOWN ? 1 : -1;
    modalList->selected = MAX(MIN(modalList->selected, modalList->entries - 1), 0);
    drawModalList(ctx, modalList);
}

void selectModalListEntry(Ctx *ctx) {
    struct ModalList *modalList = (struct ModalList *) ctx->tmpmem;
    if (modalList->onSelect && modalList->onSelect(ctx, &modalList->list[modalList->selected])) {
        toggleModalList(ctx, modalList);
    }
}
