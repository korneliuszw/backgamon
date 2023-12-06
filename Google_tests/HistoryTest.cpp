//
// Created by wired-mac on 05/12/2023.
//

extern "C" {
#include <History.h>
#include <State.h>
#include <Board.h>
}

#include <stdlib.h>
#include <gtest/gtest.h>

class HistoryTest : public testing::Test {

};

TEST_F(HistoryTest, ShouldCommitCorrectly) {
    History *history = initHistory();
    Dice dice;
    dice.rollsCount = 2;
    dice.currentRoll = 0;
    dice.rolls = (int *) malloc(sizeof(int) * 2);
    dice.rolls[0] = 2;
    Board *board = boardInit();
    Move move;
    move.from = 5;
    move.to = 3;
    start_history_entry(&history, board, &move, &dice);
    board->pts[move.from].pieces--;
    board->pts[move.to].pieces++;
    commit_history_entry(history, board, &move);
    ASSERT_EQ(history->dice->rolls[0], dice.rolls[0]);
    ASSERT_EQ(history->newToPoint->pieces, history->prevToPoint->pieces + 1);
    ASSERT_EQ(history->newFromPoint->pieces, history->prevFromPoint->pieces - 1);
}

TEST_F(HistoryTest, HistoryBackShouldRestorePreviousBoardState) {
    GameState* gameState;
    Dice dice;
    dice.rollsCount = 2;
    dice.currentRoll = 0;
    dice.rolls = (int *) malloc(sizeof(int) * 2);
    dice.rolls[0] = 2;
    History *history = new History();
    history->dice = &dice;
    history->
}
