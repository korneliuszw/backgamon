//
// Created by wired-mac on 18/11/2023.
//

extern "C" {
#include "Board.h"
}

#include <gtest/gtest.h>
#include <algorithm>
#include <stdlib.h>

class BoardTest : public testing::Test {
protected:
    void SetUp() override {
        board = boardInit();
    }

    void TearDown() override {
        free(board);
    }

    Board *board;
};

TEST_F(BoardTest, ShouldTellAllPiecesAllHome) {
    for (auto &point: board->points) {
        point.pieces = 0;
    }
    board->points[0] = {2, PLAYER_RED};
    board->points[1] = {2, PLAYER_RED};
    board->points[2] = {2, PLAYER_RED};
    board->points[3] = {2, PLAYER_RED};
    board->points[4] = {2, PLAYER_RED};
    board->points[5] = {2, PLAYER_RED};
    board->points[23] = {2, PLAYER_WHITE};
    board->points[22] = {2, PLAYER_WHITE};
    board->points[21] = {2, PLAYER_WHITE};
    board->points[20] = {2, PLAYER_WHITE};
    board->points[19] = {2, PLAYER_WHITE};
    board->points[18] = {2, PLAYER_WHITE};
    EXPECT_EQ(areAllPiecesHome(board, PLAYER_RED), true);
    EXPECT_EQ(areAllPiecesHome(board, PLAYER_WHITE), true);
}

TEST_F(BoardTest, ShouldReturnFalseIfAllPiecesArentHome) {
    for (auto &point: board->points) {
        point.pieces = 0;
    }
    board->points[0] = {2, PLAYER_RED};
    board->points[1] = {2, PLAYER_RED};
    board->points[2] = {2, PLAYER_RED};
    board->points[3] = {2, PLAYER_RED};
    board->points[4] = {2, PLAYER_RED};
    board->points[10] = {2, PLAYER_RED};
    board->points[23] = {2, PLAYER_WHITE};
    board->points[22] = {2, PLAYER_WHITE};
    board->points[21] = {2, PLAYER_WHITE};
    board->points[20] = {2, PLAYER_WHITE};
    board->points[19] = {2, PLAYER_WHITE};
    board->points[9] = {2, PLAYER_WHITE};
    EXPECT_EQ(areAllPiecesHome(board, PLAYER_RED), false);
    EXPECT_EQ(areAllPiecesHome(board, PLAYER_WHITE), false);
}

TEST_F(BoardTest, CannotMoveToPieceWithMoreThanOneOpponentPiece) {
    board->points[0] = {1, PLAYER_RED};
    board->points[1] = {2, PLAYER_WHITE};
    board->points[2] = {2, PLAYER_RED};
    board->points[3] = {1, PLAYER_WHITE};
    EXPECT_EQ(canMovePiece(board, PLAYER_RED, 0, 1), false);
    EXPECT_EQ(canMovePiece(board, PLAYER_WHITE, 2, 3), false);
}

TEST_F(BoardTest, RedCannotMoveOutOfBoundsNotLeaving) {
    EXPECT_EQ(canMovePiece(board, PLAYER_RED, BOARD_POINTS - 1, BOARD_POINTS), false);
}

TEST_F(BoardTest, WhiteCannotMoveOutOfBoundsNotLeaving) {
    EXPECT_EQ(canMovePiece(board, PLAYER_WHITE, 0, -1), false);
}

TEST_F(BoardTest, RedCanLeaveOnceAllPiecesAreInBase) {
    for (auto &point: board->points) {
        point.pieces = 0;
        point.player = 0;
    }
    board->points[0].player = PLAYER_RED;
    board->points[0].pieces = 15;
    EXPECT_EQ(canMovePiece(board, PLAYER_RED, 0, -1), true);
}

TEST_F(BoardTest, WhiteCanLeaveOnceAllPiecesAreInBase) {
    for (auto &point: board->points) {
        point.pieces = 0;
    }
    board->points[BOARD_POINTS - 1].player = PLAYER_WHITE;
    board->points[BOARD_POINTS - 1].pieces = 15;
    EXPECT_EQ(canMovePiece(board, PLAYER_WHITE, BOARD_POINTS - 1, BOARD_POINTS), true);
}


TEST_F(BoardTest, DiceRollShouldReturnTwoValuesIfNotDouble) {
    int movesCount = 0;
    srand(100);
    auto rolls = rollDice(&movesCount);
    EXPECT_EQ(movesCount, 2);
    EXPECT_EQ(rolls[0], 5);
    EXPECT_EQ(rolls[1], 4);
}

TEST_F(BoardTest, DiceRollShouldReturnFourEqualValuesIfDouble) {
    int movesCount = 0;
    srand(1);
    auto rolls = rollDice(&movesCount);
    EXPECT_EQ(movesCount, 4);
    EXPECT_EQ(rolls[0], 2);
    EXPECT_EQ(rolls[1], 2);
    EXPECT_EQ(rolls[2], 2);
    EXPECT_EQ(rolls[3], 2);
}

TEST_F(BoardTest, MovingOutALlPiecesShouldMakePlayerWinner) {
    for (int i = 0; i < 14; i++) {
        movePieceOut(board, PLAYER_RED);
    }
    EXPECT_EQ(board->winner, 0);
    movePieceOut(board, PLAYER_RED);
    EXPECT_EQ(board->winner, PLAYER_RED);
}

TEST_F(BoardTest, MoveToFriendlyPosition) {
    board->points[0] = {2, PLAYER_RED};
    board->points[1] = {1, PLAYER_RED};
    EXPECT_EQ(movePiece(board, PLAYER_RED, 1, 0), true);
    EXPECT_EQ(board->points[0].pieces, 3);
    EXPECT_EQ(board->points[1].pieces, 0);
    board->points[23] = {2, PLAYER_WHITE};
    board->points[22] = {1, PLAYER_WHITE};
    EXPECT_EQ(movePiece(board, PLAYER_WHITE, 22, 23), true);
    EXPECT_EQ(board->points[23].pieces, 3);
    EXPECT_EQ(board->points[22].pieces, 0);
}

TEST_F(BoardTest, MoveToEnemyPosition) {
    board->points[2] = {2, PLAYER_RED};
    board->points[1] = {1, PLAYER_WHITE};
    EXPECT_EQ(movePiece(board, PLAYER_RED, 2, 1), true);
    EXPECT_EQ(board->points[2].pieces, 1);
    EXPECT_EQ(board->points[1].pieces, 1);
    EXPECT_EQ(board->points[1].player, PLAYER_RED);
    EXPECT_EQ(board->bars[PLAYER_WHITE - 1].pieces, 1);
    board->points[21] = {2, PLAYER_WHITE};
    board->points[22] = {1, PLAYER_RED};
    EXPECT_EQ(movePiece(board, PLAYER_WHITE, 21, 22), true);
    EXPECT_EQ(board->points[21].pieces, 1);
    EXPECT_EQ(board->points[22].pieces, 1);
    EXPECT_EQ(board->points[22].player, PLAYER_WHITE);
    EXPECT_EQ(board->bars[PLAYER_RED - 1].pieces, 1);
}
