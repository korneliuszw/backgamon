//
// Created by wired-mac on 14/11/2023.
//


#include <Move.h>
#include <Board.h>
#include <Render.h>
#include <State.h>


#define MOVE_STEP(player) ((player) == (PR) ? (-1) : (1))

bool isValidMove(GameState *gameState, Board *board, int i) {
    return gameState->mvs.avalmvs[i].to != gameState->mvs.avalmvs[i].from &&
           board->pts[i].player == gameState->player;
}

void selectPiece(GameState *gameState, Board *board, int direction) {
    if (gameState->curpiece < 0) return;
    else if (direction == DIRECTION_DOWN) {
        for (int i = gameState->curpiece - 1; i >= 0; i--) {
            // only when 0 = 0 so this element is not an initialized move
            if (isValidMove(gameState, board, i)) {
                gameState->curpiece = i;
                gameState->update = true;
                return;
            }
        }
    } else {
        for (int i = gameState->curpiece + 1; i < BOARD_POINTS; i++) {
            // FIXME: NASTY code duplication
            // only when 0 = 0 so this element is not an initialized move
            if (isValidMove(gameState, board, i)) {
                gameState->curpiece = i;
                gameState->update = true;
                return;
            }
        }
    }
}

void makeMove(GameState *gameState, Board *board) {
    if (movePiece(board, gameState->player, gameState->mvs.avalmvs[gameState->curpiece].from,
                  gameState->mvs.avalmvs[gameState->curpiece].to))
        gameState->curpiece = gameState->mvs.avalmvs[gameState->curpiece].to;

}

void getMoves(GameState *gameState, Board *board) {
    Moves *moves = &gameState->mvs;
    if (moves->avalmvs != NULL) free(moves->avalmvs);
    gameState->curpiece = -1;
    moves->avalmvs = calloc(sizeof(int), BOARD_POINTS);
    moves->mvc = 0;
    int currentMove = getCurrentRoll(gameState) * MOVE_STEP(gameState->player);
    if (board->bars[gameState->player - 1].pieces > 0) {
        // enemy base
        gameState->curpiece = gameState->player == PR ? BOARD_POINTS - 1 : 0;
        int from = gameState->player == PR ? BOARD_POINTS : -1;
        if (canMoveToDestination(board, gameState->player, from + currentMove)) {
            moves->avalmvs[gameState->curpiece].from = from;
            moves->avalmvs[gameState->curpiece].to = from + currentMove;
            moves->mvc = 1;
        }
        return;
    }
    for (int i = 0; i < BOARD_POINTS; i++) {
        BoardPoint *boardPoint = &board->pts[i];
        if (gameState->player == boardPoint->player &&
            canMovePiece(board, gameState->player, i, i + currentMove)) {
            if (gameState->curpiece == -1) {
                gameState->curpiece = i;
            }
            moves->avalmvs[i].from = i;
            moves->avalmvs[i].to = i + currentMove;
            moves->mvc += 1;
        }
    }
    if (moves->mvc == 0) {
        transitionState(gameState, board);
    }
}
