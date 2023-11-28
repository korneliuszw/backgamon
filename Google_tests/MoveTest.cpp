//
// Created by wired-mac on 19/11/2023.
//


#include <gtest/gtest.h>

extern "C" {
#include <Move.h>
#include <State.h>
#include <Board.h>
}

class MoveTest : public testing::Test {
protected:
    void SetUp() override {
        board = boardInit();
        gameState = gameStateInit();
    }

    GameState *gameState;
    Board *board;
};

TEST_F(MoveTest, GetMoveShouldReturnCorrectMoves) {
    gameState->moves = (int *) malloc(sizeof(int) * 2);
    gameState->moves[0] = 1;
    gameState->moves[1] = 2;
    gameState->movesCount = 2;
    gameState->moveNumber = 0;
    gameState->player = PLAYER_RED;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->availableMoves.movesCount, 1 + 1 + 1);
    int realCount = 0;
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (gameState->availableMoves.availableMoves[i].from != gameState->availableMoves.availableMoves[i].to)
            realCount++;
    }
    EXPECT_EQ(realCount, gameState->availableMoves.movesCount);
}

TEST_F(MoveTest, GetMoveShouldReturnCorrectMovesForPiecesInBar) {
    gameState->moves = (int *) malloc(sizeof(int) * 2);
    gameState->moves[0] = 1;
    gameState->moves[1] = 6;
    board->bars[PLAYER_RED - 1].pieces = 1;
    board->bars[PLAYER_WHITE - 1].pieces = 1;
    gameState->player = PLAYER_WHITE;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->availableMoves.movesCount, 1);
    EXPECT_EQ(gameState->availableMoves.availableMoves[0].from, -1);
    EXPECT_EQ(gameState->availableMoves.availableMoves[0].to, 0);
    gameState->moveNumber = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->availableMoves.movesCount, 0);
    gameState->moveNumber = 0;
    gameState->player = PLAYER_RED;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->availableMoves.movesCount, 1);
    EXPECT_EQ(gameState->availableMoves.availableMoves[BOARD_POINTS - 1].from, BOARD_POINTS);
    EXPECT_EQ(gameState->availableMoves.availableMoves[BOARD_POINTS - 1].to, BOARD_POINTS - 1);
    gameState->moveNumber = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->availableMoves.movesCount, 0);
}

TEST_F(MoveTest, GetMovesShouldSkipWhenNoMoves) {
    gameState->moves = (int *) calloc(2, sizeof(int));
    for (auto &point: board->points) {
        point = {0, 0};
    }
    gameState->state = SELECTING_MOVE;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->state, ROLLING_DICE);
    gameState->moves = (int *) calloc(2, sizeof(int));
    gameState->moveNumber = 0;
    gameState->moves[0] = 5;
    gameState->player = PLAYER_WHITE;
    gameState->state = SELECTING_MOVE;
    board->points[0] = {1, PLAYER_WHITE};
    getMoves(gameState, board);
    EXPECT_NE(gameState->state, ROLLING_DICE);
    gameState->moveNumber = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->state, ROLLING_DICE);
}
