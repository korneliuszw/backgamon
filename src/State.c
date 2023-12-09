//
// Created by wired-mac on 12/11/2023.
//


#include <stdlib.h>
#include <State.h>
#include <Board.h>
#include <Render.h>
#include <string.h>
#include <time.h>

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
    ListNode *node = gs->curmove->dices->root;
    while (node != NULL) {
        Roll *roll = node->data;
        roll->enabled = false;
        node = node->next;
    }

}

void transitionMove(GameState *gameState, Board *board) {
    if (gameState->mvs.mvc > 0) {
        start_history_entry(&gameState->history, board, GMFRMPIC(gameState), gameState->dice);
        movePiece(board, gameState->player,
                  GMFRMPIC(gameState));
        useDice(gameState);
        commit_history_entry(gameState->history, board, GMFRMPIC(gameState));
    }
    BoardPoint *bp = malloc(sizeof(BoardPoint) * BOARD_POINTS);
    memcpy(bp, &board->pts, sizeof(BoardPoint) * BOARD_POINTS);
//    int p1 = board->bars[PR - 1].pieces, p2 = board->bars[PW - 1].pieces;
//    for (int i = 0; i < BOARD_POINTS; i++) {
//        if (board->pts[i].player == PW) {
//            p2 += board->pts[i].pieces;
//        } else if (board->pts[i].player == PR) {
//            p1 += board->pts[i].pieces;
//        }
//    }
//    if (p1 != 15 || p2 != 15) {
//        // noop
//        int t = 0 == 0;
//        t + 1;
//    }
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
            transitionMove(gameState, board);
            break;
    }
    gameState->update = true;
}

GameState *gameStateInit() {
    GameState *gameState = malloc(sizeof(GameState));
    gameState->state = START;
    gameState->player = 0;
    gameState->dice = malloc(sizeof(Dice));
    gameState->curpiece = -1;
    gameState->update = false;
    gameState->history = initHistory();
    gameState->timestamp = time(NULL);
    return gameState;
}
