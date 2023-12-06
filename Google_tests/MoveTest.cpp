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
    gameState->dice = (Dice *) malloc(sizeof(Dice));
    gameState->dice->rolls = (int *) malloc(sizeof(int) * 2);
    gameState->dice->rolls[0] = 1;
    gameState->dice->rolls[1] = 2;
    gameState->dice->rollsCount = 2;
    gameState->dice->currentRoll = 0;
    gameState->player = PR;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->mvs.mvc, 1 + 1 + 1);
    int realCount = 0;
    for (int i = 0; i < BOARD_POINTS; i++) {
        if (gameState->mvs.avalmvs[i].from != gameState->mvs.avalmvs[i].to)
            realCount++;
    }
    EXPECT_EQ(realCount, gameState->mvs.mvc);
}

TEST_F(MoveTest, GetMoveShouldReturnCorrectMovesForPiecesInBar) {
    gameState->dice = (Dice *) malloc(sizeof(Dice));
    gameState->dice->rolls = (int *) malloc(sizeof(int) * 2);
    gameState->dice->rolls[0] = 1;
    gameState->dice->rolls[1] = 6;
    board->bars[PR - 1].pieces = 1;
    board->bars[PW - 1].pieces = 1;
    gameState->player = PW;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->mvs.mvc, 1);
    EXPECT_EQ(gameState->mvs.avalmvs[0].from, -1);
    EXPECT_EQ(gameState->mvs.avalmvs[0].to, 0);
    gameState->dice->currentRoll = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->mvs.mvc, 0);
    gameState->dice->currentRoll = 0;
    gameState->player = PR;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->mvs.mvc, 1);
    EXPECT_EQ(gameState->mvs.avalmvs[BOARD_POINTS - 1].from, BOARD_POINTS);
    EXPECT_EQ(gameState->mvs.avalmvs[BOARD_POINTS - 1].to, BOARD_POINTS - 1);
    gameState->dice->currentRoll = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->mvs.mvc, 0);
}

TEST_F(MoveTest, GetMovesShouldSkipWhenNoMoves) {
    gameState->dice = (Dice *) malloc(sizeof(Dice));
    gameState->dice->rollsCount = 2;
    gameState->dice->rolls = (int *) malloc(sizeof(int) * 2);
    for (auto &point: board->pts) {
        point = {0, 0};
    }
    gameState->state = SELECT;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->state, DICE);
    gameState->dice->rolls = (int *) calloc(2, sizeof(int));
    gameState->dice->currentRoll = 0;
    gameState->dice->rolls[0] = 5;
    gameState->player = PW;
    gameState->state = SELECT;
    board->pts[0] = {1, PW};
    getMoves(gameState, board);
    EXPECT_NE(gameState->state, DICE);
    gameState->dice->currentRoll = 1;
    getMoves(gameState, board);
    EXPECT_EQ(gameState->state, DICE);
}
