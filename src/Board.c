#include <Board.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b)

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
        if (board->points[i].pieces > 0 && board->points[i].player == player && (i < start || i > end)) {
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
        if (moves == NULL) {
            *movesNumber = 0;
            return NULL;
        }
        moves[2] = moves[0];
        moves[3] = moves[0];
        *movesNumber = 4;
    }
    return moves;
}

bool canMoveToDestination(Board *board, int player, int to) {
    if (to == (player == PLAYER_RED ? -1 : BOARD_POINTS)) return areAllPiecesHome(board, player);
    else if (to < 0 || to >= BOARD_POINTS) return false;
    else if (board->points[to].pieces > 1 && board->points[to].player != player) return false;
    return true;
}

bool canMovePiece(Board *board, int player, int from, int to) {
    if (from < 0 || from >= BOARD_POINTS) return false;
    else if (board->bars[player - 1].pieces > 0 && from != (player == PLAYER_RED ? 0 : BOARD_POINTS - 1)) return false;
    else if (player == PLAYER_RED && from < to) return false; // clockwise movement only
    else if (player == PLAYER_WHITE && from > to) return false; // counter-clockwise movement only
    else if (board->points[from].pieces == 0) return false;
    else if (board->points[from].player != player) return false;
    return canMoveToDestination(board, player, to);
}

void movePieceOut(Board *board, int player) {
    if (board->remainingPieces[player - 1] == 1) {
        winner(board, player);
    } else board->remainingPieces[player - 1]--;
}

bool movePiece(Board *board, int player, int from, int to) {
    if (to == (player == PLAYER_RED ? BOARD_POINTS : -1)) movePieceOut(board, player);
    if (board->points[to].player != player) {
        board->points[to].pieces = 0;
        board->bars[board->points[to].player - 1].pieces++;
    }
    board->points[from].pieces = MAX(0, board->points[from].pieces - 1));
    board->points[to].pieces++;
    board->points[to].player = player;
    return true;
}


void winner(Board *board, int player) {
    board->winner = player;
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
    board->remainingPieces[PLAYER_WHITE - 1] = 15;
    board->bars[PLAYER_WHITE - 1].pieces = 0;
    board->bars[PLAYER_WHITE - 1].player = PLAYER_WHITE;
}

void seedRedPlayer(Board *board) {
    seedPoint(board, PLAYER_RED, 5, 5);
    seedPoint(board, PLAYER_RED, 7, 3);
    seedPoint(board, PLAYER_RED, 12, 5);
    seedPoint(board, PLAYER_RED, 23, 2);
    board->remainingPieces[PLAYER_RED - 1] = 15;
    board->bars[PLAYER_RED - 1].pieces = 0;
    board->bars[PLAYER_RED - 1].player = PLAYER_RED;
}

Board *boardInit() {
    sranddev();
    Board *board = malloc(sizeof(Board));
    board->winner = 0;
    for (int i = 0; i < BOARD_POINTS; i++) {
        board->points[i].pieces = 0;
        board->points[i].player = 0;
    }
    seedRedPlayer(board);
    seedWhitePlayer(board);
    return board;
}
