#include "SUS_AI.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

SUS_AI::SUS_AI(char symbol) : ai_symbol(symbol) {
    opponent_symbol = (symbol == 'S') ? 'U' : 'S';
    srand(time(0));
}

std::vector<std::pair<int, int>> SUS_AI::get_empty_cells(const std::vector<std::vector<char>>& board) const {
    std::vector<std::pair<int, int>> empty;
    for (int i = 0; i < (int)board.size() && i < 3; i++) {
        for (int j = 0; j < (int)board[i].size() && j < 3; j++) {
            if (board[i][j] == '.' || board[i][j] == ' ') {
                empty.push_back({ i, j });
            }
        }
    }
    return empty;
}

int SUS_AI::count_potential_SUS(const std::vector<std::vector<char>>& board, int x, int y, char symbol) const {
    if (board.size() < 3 || board[0].size() < 3) return 0;

    char temp_board[3][3];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp_board[i][j] = board[i][j];

    temp_board[x][y] = symbol;
    int count = 0;

    if (forms_SUS(temp_board[x][0], temp_board[x][1], temp_board[x][2]))
        count++;

    if (forms_SUS(temp_board[0][y], temp_board[1][y], temp_board[2][y]))
        count++;

    if (x == y && forms_SUS(temp_board[0][0], temp_board[1][1], temp_board[2][2]))
        count++;

    if (x + y == 2 && forms_SUS(temp_board[0][2], temp_board[1][1], temp_board[2][0]))
        count++;

    return count;
}

int SUS_AI::evaluate_move(const std::vector<std::vector<char>>& board, int x, int y, char symbol) const {
    if (board.size() < 3 || board[0].size() < 3) return 0;

    int score = 0;

    int direct_sus = count_potential_SUS(board, x, y, symbol);
    score += direct_sus * 100;

    if (x == 1 && y == 1) score += 30;
    else if ((x == 0 || x == 2) && (y == 0 || y == 2)) score += 20;
    else score += 10;

    char temp_board[3][3];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp_board[i][j] = board[i][j];
    temp_board[x][y] = symbol;

    int s_count = 0, u_count = 0, empty_count = 0;
    for (int j = 0; j < 3; j++) {
        if (temp_board[x][j] == 'S') s_count++;
        else if (temp_board[x][j] == 'U') u_count++;
        else empty_count++;
    }
    if (s_count == 2 && u_count == 1) score += 50;
    else if (s_count == 1 && u_count == 1 && empty_count == 1) score += 25;

    s_count = u_count = empty_count = 0;
    for (int i = 0; i < 3; i++) {
        if (temp_board[i][y] == 'S') s_count++;
        else if (temp_board[i][y] == 'U') u_count++;
        else empty_count++;
    }
    if (s_count == 2 && u_count == 1) score += 50;
    else if (s_count == 1 && u_count == 1 && empty_count == 1) score += 25;

    return score;
}

Move<char>* SUS_AI::get_best_move(Board<char>* board) {
    if (!board) {
        std::cerr << "ERROR: Board is null!\n";
        return nullptr;
    }

    auto matrix = board->get_board_matrix();

    if (matrix.empty() || matrix.size() < 3) {
        std::cerr << "ERROR: Board matrix is invalid!\n";
        return nullptr;
    }

    auto empty = get_empty_cells(matrix);

    if (empty.empty()) {
        std::cerr << "ERROR: No empty cells!\n";
        return nullptr;
    }

    std::pair<int, int> best_move = empty[0];
    int best_score = INT_MIN;

    for (size_t i = 0; i < empty.size(); i++) {
        int x = empty[i].first;
        int y = empty[i].second;

        int offensive_score = evaluate_move(matrix, x, y, ai_symbol);
        int defensive_value = count_potential_SUS(matrix, x, y, opponent_symbol) * 80;

        int total_score = offensive_score + defensive_value;

        if (total_score > best_score) {
            best_score = total_score;
            best_move = empty[i];
        }
    }

    return new Move<char>(best_move.first, best_move.second, ai_symbol);
}