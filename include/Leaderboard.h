//
// Created by wired-mac on 06/12/2023.
//

#ifndef BAKAGAMON_LEADERBOARD_H
#define BAKAGAMON_LEADERBOARD_H

#define MAX_LEADERS 5

typedef struct Ctx Ctx;

typedef struct PlayerInfo {
    char *name;
    int score;
} PlayerInfo;

typedef struct LeaderboardEntry {
    char *name;
    int score;
} LeaderboardEntry;

typedef struct Leaderboard {
    int leaders;
    LeaderboardEntry *entries;
} Leaderboard;

Leaderboard getLeaderboard();

void saveResult(PlayerInfo *playerInfo);

void toggleLeaderboard(Ctx *Ctx);

#endif //BAKAGAMON_LEADERBOARD_H
