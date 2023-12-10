//
// Created by wired-mac on 12/11/2023.
//


#include <stdlib.h>
#include <State.h>
#include <Board.h>
#include <Render.h>
#include <string.h>
#include <time.h>
#include <Leaderboard.h>

void transitionPickPlayer(GameState *gameState) {
    do {
        gameState->dice->rolls = rollDice(&gameState->dice->rollsCount);
    } while (gameState->dice->rollsCount == 4);
    if (gameState->dice->rolls[0].roll > gameState->dice->rolls[1].roll) gameState->player = PR;
    else gameState->player = PW;
}

void transitionDiceRoll(GameState *gameState, Board *board) {
    gameState->dice->currentRoll = 0;
    if (gameState->dice->rolls != NULL) free(gameState->dice->rolls);
    gameState->dice->rolls = rollDice(&gameState->dice->rollsCount);
    getMoves(gameState, board);
}

void useDice(GameState *gs) {
    if (gs->curmove == NULL) return;
    ListNode *node = gs->curmove->dices->root;
    while (node != NULL) {
        Roll *roll = node->data;
        roll->enabled = false;
        node = node->next;
    }

}

char *askForName() {
    char *name = malloc(sizeof(char) * 5);
    mvwprintw(getctx()->wbinf->handle, 20, 1, "Enter your name: ");
    wrefresh(getctx()->wbinf->handle);
    noraw();
    echo();
    wgetnstr(getctx()->wbinf->handle, name, 5);
    noecho();
    raw();
    if (*name == '\0') return askForName();
    return name;
}

void processVictory(GameState *gameState, Board *board) {
    PlayerInfo *playerInfo = gameState->playerInfo + board->winner - 1;
    playerInfo->score += calculateWinPoints(board);
    if (!playerInfo->name) {
        playerInfo->name = askForName();
    }
    saveResult(playerInfo);
    drwb(getctx());
    mvwprintw(getctx()->wbinf->handle, 20, 1, "Press any key to continue...");
    wrefresh(getctx()->wbinf->handle);
    gameState->state = END;
}

void transitionMove(GameState *gameState, Board *board) {
    if (gameState->mvs.mvc > 0) {
        start_history_entry(&gameState->history, gameState->player, board, GMFRMPIC(gameState), gameState->dice);
        movePiece(board, gameState->player,
                  GMFRMPIC(gameState));
        useDice(gameState);
        commit_history_entry(gameState->history, board, GMFRMPIC(gameState));
    }
    if (board->winner) return processVictory(gameState, board);
    BoardPoint *bp = malloc(sizeof(BoardPoint) * BOARD_POINTS);
    memcpy(bp, &board->pts, sizeof(BoardPoint) * BOARD_POINTS);
    gameState->update = true;
    gameState->dice->currentRoll++;
    if (gameState->dice->currentRoll < gameState->dice->rollsCount) {
        gameState->state = SELECT;
        gameState->sleep = 200; // transition delay
        getMoves(gameState, board);
    } else {
        gameState->player = gameState->player == PW ? PR : PW;
        gameState->state = DICE;
        transitionState(gameState, board);
    }
}

void endGame() {
    GameState *oldState = getctx()->gs;
    CtxInit();
    CtxInit()->gs->playerInfo = oldState->playerInfo;
}

void transitionState(GameState *gameState, Board *board) {
    switch (gameState->state) {
        case START:
            transitionPickPlayer(gameState);
            gameState->state = DICE;
            break;
        case DICE:
            transitionDiceRoll(gameState, board);
            gameState->state = SELECT;
            break;
        case RESTORE:
            getMoves(gameState, board);
            gameState->state = SELECT;
            break;
        case SELECT:
            return transitionMove(gameState, board);
        case END:
            return endGame();
    }
    gameState->update = true;
}

GameState *gameStateInit() {
    GameState *gameState = malloc(sizeof(GameState));
    gameState->state = START;
    gameState->player = 0;
    gameState->dice = malloc(sizeof(Dice));
    gameState->curpiece = -1;
    gameState->curmove = NULL;
    gameState->update = false;
    gameState->history = initHistory();
    gameState->timestamp = time(NULL);
    gameState->mvs.avalmvs = NULL;
    gameState->mvs.mvc = 0;
    gameState->playerInfo = calloc(sizeof(PlayerInfo), PLAYERS);
    return gameState;
}
