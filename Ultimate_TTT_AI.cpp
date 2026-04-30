#include "Ultimate_TTT_AI.h"
#include <algorithm>
#include <cstdlib>

UltimateTTT_AI::UltimateTTT_AI(char symbol, int depth) {
    aiSymbol = symbol;
    humanSymbol = (symbol == 'X') ? 'O' : 'X';
    maxDepth = depth;
}

UltimateTTT_AI::~UltimateTTT_AI() {}

// Main function to get best move
pair<int, pair<int, int>> UltimateTTT_AI::get_best_move(UltimateTTT_Board* board, int forcedBoard) {
    int bestScore = numeric_limits<int>::min();
    pair<int, pair<int, int>> bestMove = { 1, {0, 0} };

    vector<pair<int, pair<int, int>>> validMoves = get_valid_moves(board, forcedBoard);

    // If no valid moves (shouldn't happen), return random
    if (validMoves.empty()) {
        return { rand() % 9 + 1, {rand() % 3, rand() % 3} };
    }

    // Try each valid move
    for (auto& move : validMoves) {
        int boardNum = move.first;
        int sr = move.second.first;
        int sc = move.second.second;

        // Simulate move
        UltimateTTT_Board* simBoard = simulate_move(board, boardNum, sr, sc, aiSymbol);

        // Calculate next forced board
        int nextForced = -1;
        int idx = boardNum - 1;
        int br = idx / 3;
        int bc = idx % 3;
        char winner = simBoard->get_big_board()[br][bc];

        if (winner == '.') {
            nextForced = boardNum;
        }

        // Evaluate this move using minimax
        int score = minimax(simBoard, 0, numeric_limits<int>::min(),
            numeric_limits<int>::max(), false, nextForced);

        delete simBoard;

        // Update best move
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

// Minimax with alpha-beta pruning
int UltimateTTT_AI::minimax(UltimateTTT_Board* board, int depth, int alpha, int beta,
    bool isMaximizing, int forcedBoard) {

    // Check terminal states
    Player<char> aiPlayer("AI", aiSymbol, PlayerType::COMPUTER);
    Player<char> humanPlayer("Human", humanSymbol, PlayerType::HUMAN);
    aiPlayer.set_board_ptr(board);
    humanPlayer.set_board_ptr(board);

    if (board->is_win(&aiPlayer)) return 10000 - depth;
    if (board->is_win(&humanPlayer)) return -10000 + depth;
    if (board->is_draw(&aiPlayer)) return 0;

    // Depth limit reached
    if (depth >= maxDepth) {
        return evaluate_board(board);
    }

    char currentSymbol = isMaximizing ? aiSymbol : humanSymbol;
    vector<pair<int, pair<int, int>>> validMoves = get_valid_moves(board, forcedBoard);

    if (validMoves.empty()) return evaluate_board(board);

    if (isMaximizing) {
        int maxEval = numeric_limits<int>::min();

        for (auto& move : validMoves) {
            int boardNum = move.first;
            int sr = move.second.first;
            int sc = move.second.second;

            UltimateTTT_Board* simBoard = simulate_move(board, boardNum, sr, sc, currentSymbol);

            int nextForced = -1;
            int idx = boardNum - 1;
            int br = idx / 3;
            int bc = idx % 3;
            char winner = simBoard->get_big_board()[br][bc];
            if (winner == '.') nextForced = boardNum;

            int eval = minimax(simBoard, depth + 1, alpha, beta, false, nextForced);
            delete simBoard;

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return maxEval;

    }
    else {
        int minEval = numeric_limits<int>::max();

        for (auto& move : validMoves) {
            int boardNum = move.first;
            int sr = move.second.first;
            int sc = move.second.second;

            UltimateTTT_Board* simBoard = simulate_move(board, boardNum, sr, sc, currentSymbol);

            int nextForced = -1;
            int idx = boardNum - 1;
            int br = idx / 3;
            int bc = idx % 3;
            char winner = simBoard->get_big_board()[br][bc];
            if (winner == '.') nextForced = boardNum;

            int eval = minimax(simBoard, depth + 1, alpha, beta, true, nextForced);
            delete simBoard;

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return minEval;
    }
}

// Heuristic evaluation function
int UltimateTTT_AI::evaluate_board(UltimateTTT_Board* board) {
    int score = 0;
    auto bigBoard = board->get_big_board();

    // Evaluate big board (most important)
    // Check rows
    for (int i = 0; i < 3; i++) {
        score += score_line(bigBoard[i][0], bigBoard[i][1], bigBoard[i][2]) * 100;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        score += score_line(bigBoard[0][j], bigBoard[1][j], bigBoard[2][j]) * 100;
    }

    // Check diagonals
    score += score_line(bigBoard[0][0], bigBoard[1][1], bigBoard[2][2]) * 100;
    score += score_line(bigBoard[0][2], bigBoard[1][1], bigBoard[2][0]) * 100;

    // Bonus for center control in big board
    if (bigBoard[1][1] == aiSymbol) score += 30;
    else if (bigBoard[1][1] == humanSymbol) score -= 30;

    // Bonus for corners in big board
    int corners[4][2] = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (auto& corner : corners) {
        if (bigBoard[corner[0]][corner[1]] == aiSymbol) score += 15;
        else if (bigBoard[corner[0]][corner[1]] == humanSymbol) score -= 15;
    }

    // Evaluate small boards (less important but still matters)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (bigBoard[i][j] == '.') {
                X_O_Board* sb = board->get_small_board(i, j);
                score += score_small_board(sb, aiSymbol);
                score -= score_small_board(sb, humanSymbol);
            }
        }
    }

    return score;
}

// Score a line in big board
int UltimateTTT_AI::score_line(char a, char b, char c) {
    int aiCount = 0, humanCount = 0, emptyCount = 0;

    if (a == aiSymbol) aiCount++;
    else if (a == humanSymbol) humanCount++;
    else if (a == '.') emptyCount++;

    if (b == aiSymbol) aiCount++;
    else if (b == humanSymbol) humanCount++;
    else if (b == '.') emptyCount++;

    if (c == aiSymbol) aiCount++;
    else if (c == humanSymbol) humanCount++;
    else if (c == '.') emptyCount++;

    // Winning line
    if (aiCount == 3) return 1000;
    if (humanCount == 3) return -1000;

    // Two in a row with one empty
    if (aiCount == 2 && emptyCount == 1) return 10;
    if (humanCount == 2 && emptyCount == 1) return -10;

    // One with two empty
    if (aiCount == 1 && emptyCount == 2) return 1;
    if (humanCount == 1 && emptyCount == 2) return -1;

    return 0;
}

// Score a small board
int UltimateTTT_AI::score_small_board(X_O_Board* sb, char symbol) {
    auto mat = sb->get_board_matrix();
    int score = 0;

    // Check if close to winning
    // Rows
    for (int i = 0; i < 3; i++) {
        int count = 0;
        for (int j = 0; j < 3; j++) {
            if (mat[i][j] == symbol) count++;
        }
        if (count == 2) score += 3;
        else if (count == 1) score += 1;
    }

    // Columns
    for (int j = 0; j < 3; j++) {
        int count = 0;
        for (int i = 0; i < 3; i++) {
            if (mat[i][j] == symbol) count++;
        }
        if (count == 2) score += 3;
        else if (count == 1) score += 1;
    }

    // Center control
    if (mat[1][1] == symbol) score += 2;

    return score;
}

// Get all valid moves
vector<pair<int, pair<int, int>>> UltimateTTT_AI::get_valid_moves(UltimateTTT_Board* board, int forcedBoard) {
    vector<pair<int, pair<int, int>>> moves;

    if (forcedBoard == -1) {
        // Can play in any non-finished board
        for (int boardNum = 1; boardNum <= 9; boardNum++) {
            if (!is_small_board_finished(board, boardNum)) {
                int idx = boardNum - 1;
                int br = idx / 3;
                int bc = idx % 3;
                X_O_Board* sb = board->get_small_board(br, bc);
                auto mat = sb->get_board_matrix();

                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        if (mat[r][c] == '.') {
                            moves.push_back({ boardNum, {r, c} });
                        }
                    }
                }
            }
        }
    }
    else {
        // Must play in forced board
        if (!is_small_board_finished(board, forcedBoard)) {
            int idx = forcedBoard - 1;
            int br = idx / 3;
            int bc = idx % 3;
            X_O_Board* sb = board->get_small_board(br, bc);
            auto mat = sb->get_board_matrix();

            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    if (mat[r][c] == '.') {
                        moves.push_back({ forcedBoard, {r, c} });
                    }
                }
            }
        }
    }

    return moves;
}

// Check if small board is finished
bool UltimateTTT_AI::is_small_board_finished(UltimateTTT_Board* board, int boardNum) {
    int idx = boardNum - 1;
    int br = idx / 3;
    int bc = idx % 3;
    char status = board->get_big_board()[br][bc];
    return (status != '.');
}

// Simulate a move (returns new board)
UltimateTTT_Board* UltimateTTT_AI::simulate_move(UltimateTTT_Board* board, int boardNum, int sr, int sc, char symbol) {
    UltimateTTT_Board* newBoard = copy_board(board);
    newBoard->play_in_subboard(boardNum, sr, sc, symbol);
    return newBoard;
}

// Copy board for simulation
UltimateTTT_Board* UltimateTTT_AI::copy_board(UltimateTTT_Board* original) {
    UltimateTTT_Board* copy = new UltimateTTT_Board();

    // Copy all small boards
    for (int bi = 0; bi < 3; bi++) {
        for (int bj = 0; bj < 3; bj++) {
            X_O_Board* origSB = original->get_small_board(bi, bj);
            X_O_Board* copySB = copy->get_small_board(bi, bj);
            auto origMat = origSB->get_board_matrix();

            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    if (origMat[r][c] != '.') {
                        Move<char> m(r, c, origMat[r][c]);
                        copySB->update_board(&m);
                    }
                }
            }
        }
    }

    copy->update_big_board();
    return copy;
}