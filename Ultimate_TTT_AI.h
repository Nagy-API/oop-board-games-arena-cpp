#ifndef ULTIMATE_TTT_AI_H
#define ULTIMATE_TTT_AI_H

#include "Ultimate_TTT.h"
#include <limits>
#include <vector>
#include <utility>

using namespace std;

/*
 Smart AI Player for Ultimate Tic-Tac-Toe
 Uses Minimax with Alpha-Beta pruning and heuristic evaluation
*/

class UltimateTTT_AI {
private:
    char aiSymbol;      // 'X' or 'O'
    char humanSymbol;   // opposite of aiSymbol
    int maxDepth;       // search depth limit (3-4 is good for performance)

    // Heuristic evaluation function
    int evaluate_board(UltimateTTT_Board* board);

    // Score a 3x3 line (row/col/diag) in bigBoard
    int score_line(char a, char b, char c);

    // Score a single small board
    int score_small_board(X_O_Board* sb, char symbol);

    // Minimax with alpha-beta pruning
    int minimax(UltimateTTT_Board* board, int depth, int alpha, int beta, bool isMaximizing, int forcedBoard);

    // Get all valid moves for current state
    vector<pair<int, pair<int, int>>> get_valid_moves(UltimateTTT_Board* board, int forcedBoard);

    // Check if a small board is finished (won or tied)
    bool is_small_board_finished(UltimateTTT_Board* board, int boardNum);

    // Simulate a move and return new board state
    UltimateTTT_Board* simulate_move(UltimateTTT_Board* board, int boardNum, int sr, int sc, char symbol);

    // Copy board for simulation
    UltimateTTT_Board* copy_board(UltimateTTT_Board* original);

public:
    UltimateTTT_AI(char symbol, int depth = 3);
    ~UltimateTTT_AI();

    // Get best move for AI
    // Returns: {boardNum, {row, col}}
    pair<int, pair<int, int>> get_best_move(UltimateTTT_Board* board, int forcedBoard);
};

#endif // ULTIMATE_TTT_AI_H