//
// Created by wired-mac on 06/12/2023.
//

#ifndef BAKAGAMON_LEADERBOARD_H
#define BAKAGAMON_LEADERBOARD_H

#define MAX_LEADERS 5

typedef struct Ctx Ctx;

typedef struct LeaderboardEntry {
    char *name;
    int score;
} LeaderboardEntry;

typedef struct Leaderboard {
    int leaders;
    LeaderboardEntry entries[MAX_LEADERS];
} Leaderboard;

Leaderboard getLeaderboard();

void saveResult(char *name, int score);

void displayLeaderboard(Ctx *Ctx);

void toggleLeaderboard(Ctx *Ctx);

void moveLeaderboardCursor(Ctx *Ctx, int entry);

#endif //BAKAGAMON_LEADERBOARD_H
