//
// Created by wired-mac on 14/11/2023.
//


#include <Move.h>
#include <Board.h>
#include <Render.h>
#include <State.h>


#define MOVE_STEP(player) ((player) == (PLAYER_RED) ? (-1) : (1))

bool isValidMove(GameState *gameState, Board *board, int i) {
    return gameState->availableMoves.availableMoves[i].to != gameState->availableMoves.availableMoves[i].from &&
           board->points[i].player == gameState->player;
}

void selectPiece(GameState *gameState, Board *board, int direction) {
    if (gameState->selectedPiece < 0) return;
    else if (direction == DIRECTION_DOWN) {
        for (int i = gameState->selectedPiece - 1; i >= 0; i--) {
            // only when 0 = 0 so this element is not an initialized move
            if (isValidMove(gameState, board, i)) {
                gameState->selectedPiece = i;
                gameState->update = true;
                return;
            }
        }
    } else {
        for (int i = gameState->selectedPiece + 1; i < BOARD_POINTS; i++) {
            // FIXME: NASTY code duplication
            // only when 0 = 0 so this element is not an initialized move
            if (isValidMove(gameState, board, i)) {
                gameState->selectedPiece = i;
                gameState->update = true;
                return;
            }
        }
    }
}

void makeMove(GameState *gameState, Board *board) {
    if (movePiece(board, gameState->player, gameState->availableMoves.availableMoves[gameState->selectedPiece].from,
                  gameState->availableMoves.availableMoves[gameState->selectedPiece].to))
        gameState->selectedPiece = gameState->availableMoves.availableMoves[gameState->selectedPiece].to;

}

void getMoves(GameState *gameState, Board *board) {
    Moves *moves = &gameState->availableMoves;
    if (moves->availableMoves != NULL) free(moves->availableMoves);
    gameState->selectedPiece = -1;
    moves->availableMoves = calloc(sizeof(int), BOARD_POINTS);
    moves->movesCount = 0;
    int currentMove = gameState->moves[gameState->moveNumber] * MOVE_STEP(gameState->player);
    if (board->bars[gameState->player - 1].pieces > 0) {
        // enemy base
        gameState->selectedPiece = gameState->player == PLAYER_RED ? BOARD_POINTS - 1 : 0;
        int from = gameState->player == PLAYER_RED ? BOARD_POINTS : -1;
        if (canMoveToDestination(board, gameState->player, from + currentMove)) {
            moves->availableMoves[gameState->selectedPiece].from = from;
            moves->availableMoves[gameState->selectedPiece].to = from + currentMove;
            moves->movesCount = 1;
        }
        return;
    }
    for (int i = 0; i < BOARD_POINTS; i++) {
        BoardPoint *boardPoint = &board->points[i];
        if (gameState->player == boardPoint->player &&
            canMovePiece(board, gameState->player, i, i + currentMove)) {
            if (gameState->selectedPiece == -1) {
                gameState->selectedPiece = i;
            }
            moves->availableMoves[i].from = i;
            moves->availableMoves[i].to = i + currentMove;
            moves->movesCount += 1;
        }
    }
    if (moves->movesCount == 0) {
        transitionState(gameState, board);
    }
}