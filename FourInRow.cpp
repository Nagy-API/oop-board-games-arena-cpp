// FourInRow.cpp - LIGHTNING FAST GENIUS AI
#include "FourInRow.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

// ================= BOARD =================

FourInRow_Board::FourInRow_Board() : Board(6, 7) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool FourInRow_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char mark = toupper(move->get_symbol());
    if (col < 0 || col >= columns) return false;
    for (int r = rows - 1; r >= 0; r--) {
        if (board[r][col] == blank_symbol) {
            board[r][col] = mark;
            n_moves++;
            return true;
        }
    }
    return false;
}

bool FourInRow_Board::check_direction(int r, int c, int dr, int dc, char sym) {
    for (int k = 0; k < 4; k++) {
        int nr = r + dr * k, nc = c + dc * k;
        if (nr < 0 || nr >= rows || nc < 0 || nc >= columns || board[nr][nc] != sym)
            return false;
    }
    return true;
}

bool FourInRow_Board::is_win(Player<char>* player) {
    char sym = toupper(player->get_symbol());
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (board[r][c] == sym) {
                if (check_direction(r, c, 0, 1, sym) || check_direction(r, c, 1, 0, sym) ||
                    check_direction(r, c, 1, 1, sym) || check_direction(r, c, 1, -1, sym))
                    return true;
            }
        }
    }
    return false;
}

bool FourInRow_Board::is_draw(Player<char>* player) {
    return (n_moves == rows * columns && !is_win(player));
}

bool FourInRow_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ================= LIGHTNING FAST AI =================

FourInRow_AI::FourInRow_AI(char ai_sym, char opp_sym)
    : ai_symbol(ai_sym), opponent_symbol(opp_sym), move_count(0), time_up(false) {
    for (int i = 0; i < 10; i++) {
        killer_moves[i][0] = -1;
        killer_moves[i][1] = -1;
    }
}

bool FourInRow_AI::is_valid_column(const vector<vector<char>>& board, int col) {
    return col >= 0 && col < 7 && board[0][col] == '.';
}

int FourInRow_AI::get_next_open_row(const vector<vector<char>>& board, int col) {
    for (int r = 5; r >= 0; r--) {
        if (board[r][col] == '.') return r;
    }
    return -1;
}

vector<vector<char>> FourInRow_AI::simulate_move(const vector<vector<char>>& board, int col, char sym) {
    vector<vector<char>> new_board = board;
    int row = get_next_open_row(new_board, col);
    if (row != -1) new_board[row][col] = sym;
    return new_board;
}

string FourInRow_AI::board_hash_fast(const vector<vector<char>>& board) {
    // Ultra-fast hash using only key positions
    string h = "";
    h.reserve(42);
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            h += board[r][c];
        }
    }
    return h;
}

bool FourInRow_AI::is_time_up() {
    if (time_up) return true;
    auto now = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now - search_start).count();
    if (duration > time_limit_ms) {
        time_up = true;
        return true;
    }
    return false;
}

bool FourInRow_AI::check_win_fast(const vector<vector<char>>& board, char sym, int last_col) {
    if (last_col < 0) return false;

    int row = -1;
    for (int r = 0; r < 6; r++) {
        if (board[r][last_col] != '.') {
            row = r;
            break;
        }
    }
    if (row == -1) return false;

    // Horizontal
    int count = 1;
    for (int c = last_col - 1; c >= 0 && board[row][c] == sym; c--) count++;
    for (int c = last_col + 1; c < 7 && board[row][c] == sym; c++) count++;
    if (count >= 4) return true;

    // Vertical
    count = 1;
    for (int r = row + 1; r < 6 && board[r][last_col] == sym; r++) count++;
    if (count >= 4) return true;

    // Diagonal ↘
    count = 1;
    for (int i = 1; row - i >= 0 && last_col - i >= 0 && board[row - i][last_col - i] == sym; i++) count++;
    for (int i = 1; row + i < 6 && last_col + i < 7 && board[row + i][last_col + i] == sym; i++) count++;
    if (count >= 4) return true;

    // Diagonal ↙
    count = 1;
    for (int i = 1; row - i >= 0 && last_col + i < 7 && board[row - i][last_col + i] == sym; i++) count++;
    for (int i = 1; row + i < 6 && last_col - i >= 0 && board[row + i][last_col - i] == sym; i++) count++;
    if (count >= 4) return true;

    return false;
}

bool FourInRow_AI::is_board_full(const vector<vector<char>>& board) {
    for (int c = 0; c < 7; c++) {
        if (board[0][c] == '.') return false;
    }
    return true;
}

int FourInRow_AI::count_threats_fast(const vector<vector<char>>& board, char sym) {
    int threats = 0;

    // Quick horizontal scan
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 4; c++) {
            int cnt = 0, empty = 0;
            for (int k = 0; k < 4; k++) {
                if (board[r][c + k] == sym) cnt++;
                else if (board[r][c + k] == '.') empty++;
            }
            if (cnt == 3 && empty == 1) threats += 500;
            else if (cnt == 2 && empty == 2) threats += 20;
        }
    }

    // Quick vertical scan
    for (int c = 0; c < 7; c++) {
        for (int r = 0; r < 3; r++) {
            int cnt = 0, empty = 0;
            for (int k = 0; k < 4; k++) {
                if (board[r + k][c] == sym) cnt++;
                else if (board[r + k][c] == '.') empty++;
            }
            if (cnt == 3 && empty == 1) threats += 500;
            else if (cnt == 2 && empty == 2) threats += 20;
        }
    }

    return threats;
}

int FourInRow_AI::evaluate_fast(const vector<vector<char>>& board) {
    int score = 0;

    // Center column bonus (critical!)
    for (int r = 0; r < 6; r++) {
        if (board[r][3] == ai_symbol) score += 20;
        else if (board[r][3] == opponent_symbol) score -= 18;
    }

    // Quick threat evaluation
    score += count_threats_fast(board, ai_symbol);
    score -= count_threats_fast(board, opponent_symbol) * 1.2;

    return score;
}

vector<int> FourInRow_AI::get_valid_columns(const vector<vector<char>>& board) {
    vector<int> valid_cols;
    for (int c = 0; c < 7; c++) {
        if (is_valid_column(board, c)) valid_cols.push_back(c);
    }
    return valid_cols;
}

int FourInRow_AI::check_immediate_win(const vector<vector<char>>& board) {
    for (int col = 0; col < 7; col++) {
        if (is_valid_column(board, col)) {
            vector<vector<char>> temp = simulate_move(board, col, ai_symbol);
            if (check_win_fast(temp, ai_symbol, col)) return col;
        }
    }
    return -1;
}

int FourInRow_AI::check_immediate_threat(const vector<vector<char>>& board) {
    for (int col = 0; col < 7; col++) {
        if (is_valid_column(board, col)) {
            vector<vector<char>> temp = simulate_move(board, col, opponent_symbol);
            if (check_win_fast(temp, opponent_symbol, col)) return col;
        }
    }
    return -1;
}

bool FourInRow_AI::is_fork_move(const vector<vector<char>>& board, int col, char sym) {
    if (!is_valid_column(board, col)) return false;

    vector<vector<char>> temp = simulate_move(board, col, sym);
    int win_count = 0;

    // Count winning opportunities after this move
    for (int c = 0; c < 7; c++) {
        if (is_valid_column(temp, c)) {
            vector<vector<char>> test = simulate_move(temp, c, sym);
            if (check_win_fast(test, sym, c)) win_count++;
            if (win_count >= 2) return true;
        }
    }

    return false;
}

vector<int> FourInRow_AI::order_moves_smart(const vector<vector<char>>& board, const vector<int>& moves) {
    vector<pair<int, int>> scored;

    for (int col : moves) {
        int score = 0;

        // Immediate win = highest priority
        vector<vector<char>> temp = simulate_move(board, col, ai_symbol);
        if (check_win_fast(temp, ai_symbol, col)) {
            score += 10000000;
        }

        // Fork (double threat)
        if (is_fork_move(board, col, ai_symbol)) {
            score += 1000000;
        }

        // Center preference (very important!)
        score += (4 - abs(col - 3)) * 1000;

        scored.push_back({ score, col });
    }

    sort(scored.rbegin(), scored.rend());

    vector<int> ordered;
    for (auto& p : scored) ordered.push_back(p.second);
    return ordered;
}

int FourInRow_AI::minimax_lightning(const vector<vector<char>>& board, int depth, int alpha, int beta, bool maximizing, int last_col) {
    // Time check (every few nodes)
    if (depth % 2 == 0 && is_time_up()) {
        return evaluate_fast(board);
    }

    // Check TT (lightweight)
    if (depth <= 5) {
        string hash = board_hash_fast(board);
        if (tt_cache.find(hash) != tt_cache.end()) {
            return tt_cache[hash];
        }
    }

    // Terminal checks
    if (last_col >= 0) {
        if (check_win_fast(board, ai_symbol, last_col)) return WIN_SCORE + depth;
        if (check_win_fast(board, opponent_symbol, last_col)) return LOSE_SCORE - depth;
    }

    if (depth == 0 || is_board_full(board)) {
        return evaluate_fast(board);
    }

    vector<int> valid_cols = get_valid_columns(board);
    valid_cols = order_moves_smart(board, valid_cols);

    if (maximizing) {
        int max_eval = numeric_limits<int>::min();

        for (int col : valid_cols) {
            vector<vector<char>> new_board = simulate_move(board, col, ai_symbol);
            int eval = minimax_lightning(new_board, depth - 1, alpha, beta, false, col);

            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) {
                // Store killer move
                if (depth < 10 && killer_moves[depth][0] != col) {
                    killer_moves[depth][1] = killer_moves[depth][0];
                    killer_moves[depth][0] = col;
                }
                break;
            }
        }

        // Store in TT (limit size)
        if (depth <= 5 && tt_cache.size() < 50000) {
            string hash = board_hash_fast(board);
            tt_cache[hash] = max_eval;
        }

        return max_eval;
    }
    else {
        int min_eval = numeric_limits<int>::max();

        for (int col : valid_cols) {
            vector<vector<char>> new_board = simulate_move(board, col, opponent_symbol);
            int eval = minimax_lightning(new_board, depth - 1, alpha, beta, true, col);

            min_eval = min(min_eval, eval);
            beta = min(beta, eval);

            if (beta <= alpha) break;
        }

        // Store in TT
        if (depth <= 5 && tt_cache.size() < 50000) {
            string hash = board_hash_fast(board);
            tt_cache[hash] = min_eval;
        }

        return min_eval;
    }
}

int FourInRow_AI::iterative_deepening_search(const vector<vector<char>>& board) {
    int best_col = 3; // Default to center
    int best_score = numeric_limits<int>::min();

    // Iterative deepening: start shallow, go deeper (SILENT MODE)
    for (int depth = 4; depth <= MAX_DEPTH && !is_time_up(); depth += 2) {
        vector<int> valid_cols = get_valid_columns(board);
        valid_cols = order_moves_smart(board, valid_cols);

        int depth_best_col = best_col;
        int depth_best_score = numeric_limits<int>::min();

        for (int col : valid_cols) {
            if (is_time_up()) break;

            vector<vector<char>> temp = simulate_move(board, col, ai_symbol);
            int score = minimax_lightning(temp, depth - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, col);

            if (score > depth_best_score) {
                depth_best_score = score;
                depth_best_col = col;
            }
        }

        // Update best move from this depth (completely silent)
        if (!is_time_up()) {
            best_col = depth_best_col;
            best_score = depth_best_score;
            // No output at all!
        }
    }

    return best_col;
}

int FourInRow_AI::get_best_move(const vector<vector<char>>& board) {
    // Reset time management
    search_start = chrono::high_resolution_clock::now();
    time_up = false;

    // Opening book
    if (move_count == 0) {
        return 3;
    }

    // Immediate win
    int win_col = check_immediate_win(board);
    if (win_col != -1) {
        return win_col;
    }

    // Fork opportunity
    for (int col = 0; col < 7; col++) {
        if (is_fork_move(board, col, ai_symbol)) {
            return col;
        }
    }

    // Block immediate threat
    int threat_col = check_immediate_threat(board);
    if (threat_col != -1) {
        return threat_col;
    }

    // Block opponent's fork
    for (int col = 0; col < 7; col++) {
        if (is_fork_move(board, col, opponent_symbol)) {
            return col;
        }
    }

    // Iterative deepening search
    int best_col = iterative_deepening_search(board);

    return best_col;
}

// ================= UI =================

FourInRow_UI::FourInRow_UI()
    : UI<char>("Welcome to Four-in-a-Row!", 3),
    ai_player1(nullptr), ai_player2(nullptr) {
}

FourInRow_UI::~FourInRow_UI() {
    delete ai_player1;
    delete ai_player2;
}

Player<char>* FourInRow_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        cout << "Creating AI player: " << name << " (" << symbol << ")\n";

        char opponent = (symbol == 'X') ? 'O' : 'X';

        if (symbol == 'X') {
            ai_player1 = new FourInRow_AI(toupper(symbol), opponent);
        }
        else {
            ai_player2 = new FourInRow_AI(toupper(symbol), opponent);
        }
    }
    else {
        cout << "Creating human player: " << name << " (" << symbol << ")\n";
    }

    return new Player<char>(name, toupper(symbol), type);
}

int FourInRow_UI::ask_column() {
    int col;
    cout << "Enter column (0-6): ";
    cin >> col;
    return col;
}

Move<char>* FourInRow_UI::get_move(Player<char>* player) {
    int col;

    if (player->get_type() == PlayerType::HUMAN) {
        col = ask_column();
    }
    else {
        FourInRow_AI* ai = (player->get_symbol() == 'X') ? ai_player1 : ai_player2;

        if (ai) {
            col = ai->get_best_move(player->get_board_ptr()->get_board_matrix());
            ai->increment_move_count();
        }
        else {
            col = rand() % 7;
        }
    }

    return new Move<char>(0, col, player->get_symbol());
}
