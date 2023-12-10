//
// Created by wired-mac on 06/12/2023.
//

#include <Leaderboard.h>
#include <stdio.h>
#include <Render.h>
#include <ModalList.h>

Leaderboard getLeaderboard() {
    Leaderboard leaderboard;
    leaderboard.leaders = 0;
    leaderboard.entries = malloc(sizeof(LeaderboardEntry) * MAX_LEADERS);
    FILE *file = fopen("states.b", "r");
    if (file == NULL) {
        return leaderboard;
    }
    while (!feof(file) && leaderboard.leaders < MAX_LEADERS) {
        char *name = malloc(sizeof(char) * 20);
        int score;
        fscanf(file, "%19s", name);
        fscanf(file, "%d\n", &score);
        leaderboard.entries[leaderboard.leaders].name = name;
        leaderboard.entries[leaderboard.leaders].score = score;
        leaderboard.leaders++;
    }
    return leaderboard;
}

void placeLeader(Leaderboard *leaderboard, char *name, int score) {
    for (int i = 0; i < MAX_LEADERS; i++) {
        if (leaderboard->entries[i].score < score) {
            for (int j = leaderboard->leaders - 1; j > i; j--) {
                leaderboard->entries[j] = leaderboard->entries[j - 1];
            }
            leaderboard->entries[i].name = name;
            leaderboard->entries[i].score = score;
            if (leaderboard->leaders < MAX_LEADERS)
                leaderboard->leaders++;
            return;
        }
    }
}

void saveResult(PlayerInfo *pi) {
    Leaderboard leaderboard = getLeaderboard();
    placeLeader(&leaderboard, pi->name, pi->score);
    FILE *file = fopen("states.b", "w");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < leaderboard.leaders; i++) {
        fprintf(file, "%s %d\n", leaderboard.entries[i].name, leaderboard.entries[i].score);
    }
    fclose(file);
}

void initLeaderboardScreen(struct ModalList *list) {
    Leaderboard ldb = getLeaderboard();
    list->selected = 0;
    list->entries = ldb.leaders;
    list->onSelect = NULL;
    list->list = malloc(sizeof(struct ModalListEntry) * ldb.leaders);
    for (int i = 0; i < ldb.leaders; i++) {
        char *text = malloc(sizeof(char) * 100);
        snprintf(text, 100, "%d) %s: %d", i + 1, ldb.entries[i].name, ldb.entries[i].score);
        list->list[i].data = NULL;
        list->list[i].text = text;
    }
}

void toggleLeaderboard(Ctx *ctx) {
    struct ModalList *list;
    if (ctx->curwin != LEADER_WINDOW) {
        ctx->curwin = LEADER_WINDOW;
        list = malloc(sizeof(struct ModalList));
        initLeaderboardScreen(list);
    } else {
        list = ctx->tmpmem;
    }
    toggleModalList(ctx, list);
}
