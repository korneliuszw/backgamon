//
// Created by wired-mac on 12/11/2023.
//


#include <stdlib.h>
#include <State.h>
#include <Board.h>
#include <Render.h>

void transitionPickPlayer(GameState *gameState) {
    do {
        gameState->dice->rolls = rollDice(&gameState->dice->rollsCount);
    } while (gameState->dice->rollsCount == 4);
    if (gameState->dice->rolls[0].roll > gameState->dice->rolls[1].roll) gameState->player = PR;
    else gameState->player = PW;
}

void transitionDiceRoll(GameState *gameState, Board *board) {
    gameState->dice->currentRoll = 0;
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
                  GMFRMPIC(gameState)->from,
                  GMFRMPIC(gameState)->to);
        useDice(gameState);
        commit_history_entry(gameState->history, board, GMFRMPIC(gameState));
    }
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
    return gameState;
}
