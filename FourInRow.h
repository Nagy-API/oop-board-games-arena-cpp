// FourInRow.h
//AI done
#ifndef FOUR_IN_ROW_H
#define FOUR_IN_ROW_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <cctype>
#include <limits>
#include <unordered_map>
#include <chrono>
using namespace std;

class FourInRow_Board : public Board<char> {
private:
    char blank_symbol = '.';
    bool check_direction(int r, int c, int dr, int dc, char sym);

public:
    FourInRow_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

// ==================== LIGHTNING FAST GENIUS AI ====================
class FourInRow_AI {
private:
    char ai_symbol;
    char opponent_symbol;
    int move_count;

    // Lightweight data structures
    int killer_moves[10][2];
    unordered_map<string, int> tt_cache; // Simplified TT

    // Time management
    chrono::time_point<chrono::high_resolution_clock> search_start;
    int time_limit_ms = 1500; // 1.5 seconds max
    bool time_up = false;

    static const int WIN_SCORE = 10000000;
    static const int LOSE_SCORE = -10000000;
    static const int MAX_DEPTH = 9;

    // Core functions
    bool is_valid_column(const vector<vector<char>>& board, int col);
    int get_next_open_row(const vector<vector<char>>& board, int col);
    vector<vector<char>> simulate_move(const vector<vector<char>>& board, int col, char sym);
    bool check_win_fast(const vector<vector<char>>& board, char sym, int last_col);
    bool is_board_full(const vector<vector<char>>& board);
    string board_hash_fast(const vector<vector<char>>& board);
    bool is_time_up();

    // Super fast evaluation
    int evaluate_fast(const vector<vector<char>>& board);
    int count_threats_fast(const vector<vector<char>>& board, char sym);

    // Strategic analysis
    vector<int> get_valid_columns(const vector<vector<char>>& board);
    vector<int> order_moves_smart(const vector<vector<char>>& board, const vector<int>& moves);
    int check_immediate_win(const vector<vector<char>>& board);
    int check_immediate_threat(const vector<vector<char>>& board);
    bool is_fork_move(const vector<vector<char>>& board, int col, char sym);

    // Lightning fast minimax
    int minimax_lightning(const vector<vector<char>>& board, int depth, int alpha, int beta, bool maximizing, int last_col);

    // Iterative deepening
    int iterative_deepening_search(const vector<vector<char>>& board);

public:
    FourInRow_AI(char ai_sym, char opp_sym);
    int get_best_move(const vector<vector<char>>& board);
    void increment_move_count() { move_count++; }
};

class FourInRow_UI : public UI<char> {
private:
    FourInRow_AI* ai_player1;
    FourInRow_AI* ai_player2;
    int ask_column();

public:
    FourInRow_UI();
    ~FourInRow_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif // FOUR_IN_ROW_H
