//
// Created by wired-mac on 18/11/2023.
//

extern "C" {
#include "Board.h"
}

#include <gtest/gtest.h>
#include <algorithm>

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