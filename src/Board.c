#include <Board.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "State.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b)

int getPlayerBoardStart(int player) {
    if (player == PR) return 0;
    return 18;
}

int getPlayerBoardEnd(int player) {
    if (player == PR) return 5;
    return 23;
}

bool areAllPiecesHome(Board *board, int player) {
    int end = getPlayerBoardEnd(player);
    int start = getPlayerBoardStart(player);
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (board->pts[i].pieces > 0 && board->pts[i].player == player && (i < start || i > end)) {
            return false;
        }
    }
    return true;
}

Roll throw_dice() {
    int rng = rand() % 6 + 1;
    Roll roll = {rng, true};
    return roll;
}

// TODO: Maybe switch to unsigned char
Roll *rollDice(int *movesNumber) {
    Roll *moves = malloc(sizeof(Roll) * 2);
    *movesNumber = 2;
    moves[0] = throw_dice();
    moves[1] = throw_dice();
    if (moves[0].roll == moves[1].roll) {
        moves = realloc(moves, sizeof(Roll) * 4);
        moves[2] = moves[0];
        moves[3] = moves[0];
        *movesNumber = 4;
    }
    return moves;
}

bool canMoveToDestination(Board *board, int player, int to) {
    if (to == (player == PR ? -1 : BOARD_POINTS)) return areAllPiecesHome(board, player);
    else if (to < 0 || to >= BOARD_POINTS) return false;
    else if (board->pts[to].pieces > 1 && board->pts[to].player != player) return false;
    return true;
}

bool canMovePiece(Board *board, int player, int from, int to) {
    if (from < 0 || from >= BOARD_POINTS) return false;
    else if (board->bars[player - 1].pieces > 0 && from != (player == PR ? 0 : BOARD_POINTS - 1)) return false;
    else if (player == PR && from < to) return false; // clockwise movement only
    else if (player == PW && from > to) return false; // counter-clockwise movement only
    else if (board->pts[from].pieces == 0) return false;
    else if (board->pts[from].player != player) return false;
    return canMoveToDestination(board, player, to);
}

void movePieceOut(Board *board, int player, Move *mv) {
    board->pts[mv->from].pieces = MAX(0, board->pts[mv->from].pieces - 1));
    if (board->rempic[player - 1] == 1) {
        winner(board, player);
    } else board->rempic[player - 1]--;
}

#define IS_BAND_MOVE(b, p) (b)->bars[p - 1].pieces > 0

void movePieceOutOfBand(Board *board, int player, Move *mv) {
    board->bars[player - 1].pieces--;
}

bool movePiece(Board *board, int player, Move *mv) {
    if (mv->to == (player == PR ? -1 : BOARD_POINTS)) {
        movePieceOut(board, player, mv);
        return true;
    }
    if (IS_BAND_MOVE(board, player)) movePieceOutOfBand(board, player, mv);
    else board->pts[mv->from].pieces = MAX(0, board->pts[mv->from].pieces - 1));
    if (board->pts[mv->to].player != player && board->pts[mv->to].player > 0) {
        if (board->pts[mv->to].pieces > 0)
            board->bars[board->pts[mv->to].player - 1].pieces++;
        board->pts[mv->to].pieces = 0;
    }
    board->pts[mv->to].pieces++;
    board->pts[mv->to].player = player;
    return true;
}

int calculateWinPoints(Board *board) {
    int otherPlayer = board->winner == PR ? PW : PR;
    int points = 1;
    if (!areAllPiecesHome(board, otherPlayer)) points = 2;
    if (board->bars[otherPlayer - 1].pieces > 0) points = 3;
    return points;
}


void winner(Board *board, int player) {
    board->winner = player;
    calculateWinPoints(board);
}

void seedPoint(Board *board, int player, int point, int pieces) {
    board->pts[point].player = player;
    board->pts[point].pieces = pieces;
}

void seedWhitePlayer(Board *board) {
    seedPoint(board, PW, 0, 2);
    seedPoint(board, PW, 11, 5);
    seedPoint(board, PW, 16, 3);
    seedPoint(board, PW, 18, 5);
    board->rempic[PW - 1] = 15;
    board->bars[PW - 1].pieces = 0;
    board->bars[PW - 1].player = PW;
}

void seedRedPlayer(Board *board) {
    seedPoint(board, PR, 5, 5);
    seedPoint(board, PR, 7, 3);
    seedPoint(board, PR, 12, 5);
    seedPoint(board, PR, 23, 2);
    board->rempic[PR - 1] = 15;
    board->bars[PR - 1].pieces = 0;
    board->bars[PR - 1].player = PR;
}

Board *boardInit() {
    srand(time(NULL));
    Board *board = malloc(sizeof(Board));
    board->winner = 0;
    for (int i = 0; i < BOARD_POINTS; i++) {
        board->pts[i].pieces = 0;
        board->pts[i].player = 0;
    }
    seedRedPlayer(board);
    seedWhitePlayer(board);
    return board;
}
