//
// Created by wired-mac on 12/11/2023.
//


#include <stdlib.h>
#include <State.h>
#include <Board.h>
#include <Render.h>

void transitionPickPlayer(GameState *gameState) {
    do {
        gameState->moves = rollDice(&gameState->movesCount);
    } while (gameState->movesCount == 4);
    if (gameState->moves[0] > gameState->moves[1]) gameState->player = PLAYER_RED;
    else gameState->player = PLAYER_WHITE;
}

void transitionDiceRoll(GameState *gameState, Board *board) {
    if (gameState->moves != NULL) free(gameState->moves);
    gameState->moveNumber = 0;
    gameState->moves = rollDice(&gameState->movesCount);
    getMoves(gameState, board);
}

void transitionMove(GameState *gameState, Board *board) {
    if (gameState->availableMoves.movesCount > 0)
        movePiece(board, gameState->player,
                  GET_MOVE_FROM_SELECTED_PIECE(gameState).from,
                  GET_MOVE_FROM_SELECTED_PIECE(gameState).to);
    gameState->update = true;
    gameState->moveNumber++;
    if (gameState->moveNumber < gameState->movesCount) {
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
    gameState->moves = NULL;
    gameState->movesCount = 0;
    gameState->moveNumber = 0;
    gameState->selectedPiece = -1;
    gameState->update = false;
    return gameState;
}

