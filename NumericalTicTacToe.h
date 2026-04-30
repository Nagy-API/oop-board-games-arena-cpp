// NumericalTicTacToe.h
// Numerical Tic-Tac-Toe (3x3) - header
#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Numerical Tic-Tac-Toe rules (as required):
 * - 3x3 board of integers
 * - Player 1 uses odd numbers: 1,3,5,7,9
 * - Player 2 uses even numbers: 2,4,6,8
 * - Each number can be used only once
 * - A player wins when any row/col/diag sums exactly to 15
 */

class NumericalTTT_Board : public Board<int> {
private:
    const int blank_symbol = 0;            // empty cell is 0
    set<int> used_numbers;                 // numbers already placed on the board

    // helper: check whether the three positions (by values) sum to 15
    bool three_sum_is_15(int a, int b, int c) const;

public:
    NumericalTTT_Board();                  // default 3x3 constructor

    // core overrides from Board<int>
    bool update_board(Move<int>* move) override;       // apply a move (x,y,value)
    bool is_win(Player<int>* player) override;         // check if player has a 3-line summing to 15
    bool is_lose(Player<int>* player) override;        // not used: always false here
    bool is_draw(Player<int>* player) override;        // draw if board full and no winner
    bool game_is_over(Player<int>* player) override;   // true if win or draw
};


class NumericalTTT_UI : public UI<int> {
public:
    NumericalTTT_UI();

    ~NumericalTTT_UI() {}

    // create_player: name, symbol (we'll store '1' or '2' as ints for identification),
    // and type (HUMAN or COMPUTER)
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;

    // get_move: ask human or generate/computer move
    Move<int>* get_move(Player<int>* player) override;

private:
    // helper to ask user for a number choice (validate available and parity)
    int ask_number_from_player(Player<int>* player);

    // helper to ask for coordinates (x,y)
    pair<int, int> ask_coordinates();
};

#endif // NUMERICAL_TICTACTOE_H