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
    if (gameState->dice->rolls[0] > gameState->dice->rolls[1]) gameState->player = PLAYER_RED;
    else gameState->player = PLAYER_WHITE;
}

void transitionDiceRoll(GameState *gameState, Board *board) {
    gameState->dice->currentRoll = 0;
    gameState->dice->rolls = rollDice(&gameState->dice->rollsCount);
    getMoves(gameState, board);
}

void transitionMove(GameState *gameState, Board *board) {
    if (gameState->availableMoves.movesCount > 0) {
        start_history_entry(&gameState->history, board, &GET_MOVE_FROM_SELECTED_PIECE(gameState), gameState->dice);
        movePiece(board, gameState->player,
                  GET_MOVE_FROM_SELECTED_PIECE(gameState).from,
                  GET_MOVE_FROM_SELECTED_PIECE(gameState).to);
        commit_history_entry(gameState->history, board, &GET_MOVE_FROM_SELECTED_PIECE(gameState));
    }
    gameState->update = true;
    gameState->dice->currentRoll++;
    if (gameState->dice->currentRoll < gameState->dice->rollsCount) {
        gameState->state = SELECTING_MOVE;
        gameState->sleep = 200; // transition delay
        getMoves(gameState, board);
    } else {
        gameState->player = gameState->player == PLAYER_WHITE ? PLAYER_RED : PLAYER_WHITE;
        gameState->state = ROLLING_DICE;
        transitionState(gameState, board);
    }
}


void transitionState(GameState *gameState, Board *board) {
    switch (gameState->state) {
        case PICKING_PLAYER:
            transitionPickPlayer(gameState);
            gameState->state = ROLLING_DICE;
            break;
        case ROLLING_DICE:
            transitionDiceRoll(gameState, board);
            gameState->state = SELECTING_MOVE;
            break;
        case SELECTING_MOVE:
            transitionMove(gameState, board);
            break;
    }
    gameState->update = true;
}

GameState *gameStateInit() {
    GameState *gameState = malloc(sizeof(GameState));
    gameState->state = PICKING_PLAYER;
    gameState->player = 0;
    gameState->dice = malloc(sizeof(Dice));
    gameState->selectedPiece = -1;
    gameState->update = false;
    gameState->history = initHistory();
    return gameState;
}

int getCurrentRoll(GameState *gameState) {
    if (gameState->dice->rollsCount <= gameState->dice->currentRoll) exit(1);
    return gameState->dice->rolls[gameState->dice->currentRoll];
}
