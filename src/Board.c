#include <Board.h>
#include <stdlib.h>
#include <stdbool.h>

int getPlayerBoardStart(int player) {
    if (player == PLAYER_RED) return 0;
    return 18;
}

int getPlayerBoardEnd(int player) {
    if (player == PLAYER_RED) return 5;
    return 23;
}

bool areAllPiecesHome(Board *board, int player) {
    int end = getPlayerBoardEnd(player);
    int start = getPlayerBoardStart(player);
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (board->points[i].player == player && (i < start || i > end)) {
            return false;
        }
    }
    return true;
}

int throw_dice() {
    return rand() % 6 + 1;
}

// TODO: Maybe switch to unsigned char
int *rollDice(int *movesNumber) {
    int *moves = malloc(sizeof(int) * 2);
    *movesNumber = 2;
    moves[0] = throw_dice();
    moves[1] = throw_dice();
    if (moves[0] == moves[1]) {
        moves = realloc(moves, sizeof(int) * 4);
        moves[2] = moves[0];
        moves[3] = moves[0];
        *movesNumber = 4;
    }
    return moves;
}

bool canMovePiece(Board *board, int player, int from, int to) {
    if (from < 0 || from >= BOARD_POINTS || to < 0 || to >= BOARD_POINTS) return false;
    else if (board->bars[player - 1].pieces > 0 && from != (player == PLAYER_RED ? 0 : BOARD_POINTS - 1)) return false;
    else if (player == PLAYER_RED && from < to) return false; // clockwise movement only
    else if (player == PLAYER_WHITE && from > to) return false; // counter-clockwise movement only
    else if (board->points[from].pieces == 0) return false;
    else if (board->points[from].player != player) return false;
    else if (board->points[to].pieces > 1 && board->points[to].player != player) return false;
    return true;
}

bool movePiece(Board *board, int player, int from, int to) {
    if (board->points[from].player != player) return false;
    if (board->points[to].player != player) {
        if (board->points[to].pieces > 1) return false;
        board->points[to].pieces = 0;
        board->bars[board->points[to].player - 1].pieces++;
    }
    board->points[from].pieces--;
    board->points[to].pieces++;
    board->points[to].player = player;
    return true;
}


int winner(Board *board) {
    if (areAllPiecesHome(board, PLAYER_RED)) return PLAYER_RED;
    if (areAllPiecesHome(board, PLAYER_WHITE)) return PLAYER_WHITE;
    return 0;
}

void seedPoint(Board *board, int player, int point, int pieces) {
    board->points[point].player = player;
    board->points[point].pieces = pieces;
}

void seedWhitePlayer(Board *board) {
    seedPoint(board, PLAYER_WHITE, 0, 2);
    seedPoint(board, PLAYER_WHITE, 11, 5);
    seedPoint(board, PLAYER_WHITE, 16, 3);
    seedPoint(board, PLAYER_WHITE, 18, 5);
}

void seedRedPlayer(Board *board) {
    seedPoint(board, PLAYER_RED, 5, 5);
    seedPoint(board, PLAYER_RED, 7, 3);
    seedPoint(board, PLAYER_RED, 12, 5);
    seedPoint(board, PLAYER_RED, 23, 2);
}

Board *boardInit() {
    sranddev();
    Board *board = malloc(sizeof(Board));
    for (int i = 0; i < BOARD_POINTS; i++) {
        board->points[i].pieces = 0;
        board->points[i].player = 0;
    }
    seedRedPlayer(board);
    seedWhitePlayer(board);
    return board;
}
