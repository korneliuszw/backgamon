//
// Created by wired-mac on 06/12/2023.
//

#include <Leaderboard.h>
#include <stdio.h>
#include <Render.h>

typedef struct LeaderboardState {
    Leaderboard leaderboard;
    int selectedEntry;
} LeaderboardState;

Leaderboard getLeaderboard() {
    Leaderboard leaderboard;
    leaderboard.leaders = 0;
    FILE *file = fopen("states.b", "r");
    if (file == NULL) {
        return leaderboard;
    }
    while (!feof(file) && leaderboard.leaders < MAX_LEADERS) {
        char name[100];
        int score;
        fscanf(file, "%99s %d\n", name, &score);
        leaderboard.entries[leaderboard.leaders].name = name;
        leaderboard.entries[leaderboard.leaders].score = score;
        leaderboard.leaders++;
    }
    return leaderboard;
}

void placeLeader(Leaderboard *leaderboard, char *name, int score) {
    for (int i = 0; i < leaderboard->leaders; i++) {
        if (leaderboard->entries[i].score < score) {
            for (int j = leaderboard->leaders - 1; j > i; j--) {
                leaderboard->entries[j] = leaderboard->entries[j - 1];
            }
            leaderboard->entries[i].name = name;
            leaderboard->entries[i].score = score;
            return;
        }
    }
}

void saveResult(char *name, int score) {
    Leaderboard leaderboard = getLeaderboard();
    placeLeader(&leaderboard, name, score);
    FILE *file = fopen("states.b", "w");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < leaderboard.leaders; i++) {
        fprintf(file, "%s %d\n", leaderboard.entries[i].name, leaderboard.entries[i].score);
    }
    fclose(file);
}

void renderLeaderboard(WindowInfo *windowInfo, LeaderboardState *state) {
    for (int i = 0; i < state->leaderboard.leaders; i++) {
        USE_COLOR(i == state->selectedEntry, COLOR_PAIR_WHITE_PLAYER_SELECTED, windowInfo->handle) {
                mvwprintw(windowInfo->handle, i + 1, 1, "%d) %s %d", i, state->leaderboard.entries[i].name,
                          state->leaderboard.entries[i].score);
            }
    }
    wrefresh(windowInfo->handle);
}

void moveLeaderboardCursor(Context *context, int entry) {
    LeaderboardState *state = (LeaderboardState *) context->workingMemory;
    if (entry > 0) {
        state->selectedEntry = (state->selectedEntry + 1) % state->leaderboard.leaders;
    } else {
        state->selectedEntry = (state->selectedEntry - 1 + state->leaderboard.leaders) %
                               state->leaderboard.leaders;
    }
    renderLeaderboard(context->modalWindow, context->workingMemory);
}

void displayLeaderboard(Context *context) {
    context->currentWindow = LEADER_WINDOW;
    context->modalWindow = createWindow(10, 10, false, context->windowInfo->width / 2 - 5,
                                        context->windowInfo->height / 2 - 5);
    context->workingMemory = calloc(1, sizeof(LeaderboardState));
    ((LeaderboardState *) (context->workingMemory))->leaderboard = getLeaderboard();
    renderLeaderboard(context->modalWindow, context->workingMemory);
}

void toggleLeaderboard(Context *context) {
    if (context->currentWindow == LEADER_WINDOW) {
        context->currentWindow = MAIN_WINDOW;
        free(context->workingMemory);
        delwin(context->modalWindow->handle);
        free(context->modalWindow);
        context->modalWindow = NULL;
    } else {
        displayLeaderboard(context);
    }
}
