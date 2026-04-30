#include "5x5 Tic Tac Toe.h"
#include <iostream>
#include <vector>
#include <cctype>
#include <utility>

using namespace std;

Five_TTT_Board::Five_TTT_Board()
    : Board<char>(5, 5)
{
    for (auto& row : board)
        for (auto& c : row)
            c = blank_symbol;
}

bool Five_TTT_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= 5 || y < 0 || y >= 5)
        return false;

    if (board[x][y] != blank_symbol)
        return false;

    board[x][y] = static_cast<char>(toupper(sym));
    n_moves++;
    return true;
}

int Five_TTT_Board::countTriples(char s) const
{
    int count = 0;
    s = static_cast<char>(toupper(s));

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == s && board[i][j + 1] == s && board[i][j + 2] == s)
                count++;

    for (int j = 0; j < 5; j++)
        for (int i = 0; i < 3; i++)
            if (board[i][j] == s && board[i + 1][j] == s && board[i + 2][j] == s)
                count++;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == s && board[i + 1][j + 1] == s && board[i + 2][j + 2] == s)
                count++;

    for (int i = 2; i < 5; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == s && board[i - 1][j + 1] == s && board[i - 2][j + 2] == s)
                count++;

    return count;
}

bool Five_TTT_Board::game_is_over(Player<char>*)
{
    return n_moves == 24;
}

bool Five_TTT_Board::is_win(Player<char>* player)
{
    if (!game_is_over(player)) return false;

    char sym = static_cast<char>(toupper(player->get_symbol()));
    char other = (sym == 'X' ? 'O' : 'X');

    return countTriples(sym) > countTriples(other);
}

bool Five_TTT_Board::is_lose(Player<char>* player)
{
    if (!game_is_over(player)) return false;

    char sym = static_cast<char>(toupper(player->get_symbol()));
    char other = (sym == 'X' ? 'O' : 'X');

    return countTriples(sym) < countTriples(other);
}

bool Five_TTT_Board::is_draw(Player<char>* player)
{
    if (!game_is_over(player)) return false;

    char sym = static_cast<char>(toupper(player->get_symbol()));
    char other = (sym == 'X' ? 'O' : 'X');

    return countTriples(sym) == countTriples(other);
}

Five_TTT_UI::Five_TTT_UI()
    : UI<char>("Welcome to 5x5 Tic Tac Toe", 2)
{
}

Player<char>* Five_TTT_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

pair<int, int> find_best_ai_move(Board<char>* b, char ai, char human)
{
    auto mat = b->get_board_matrix();

    auto is_valid = [&](int x, int y) {
        return x >= 0 && x < 5 && y >= 0 && y < 5 && mat[x][y] == '.';
        };

    auto count_triples_in_matrix = [](const vector<vector<char>>& m, char s) {
        int count = 0;

        for (int r = 0; r < 5; r++)
            for (int c = 0; c < 3; c++)
                if (m[r][c] == s && m[r][c + 1] == s && m[r][c + 2] == s)
                    count++;

        for (int c = 0; c < 5; c++)
            for (int r = 0; r < 3; r++)
                if (m[r][c] == s && m[r + 1][c] == s && m[r + 2][c] == s)
                    count++;

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (m[r][c] == s && m[r + 1][c + 1] == s && m[r + 2][c + 2] == s)
                    count++;

        for (int r = 2; r < 5; r++)
            for (int c = 0; c < 3; c++)
                if (m[r][c] == s && m[r - 1][c + 1] == s && m[r - 2][c + 2] == s)
                    count++;

        return count;
        };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (mat[i][j] == '.') {
                mat[i][j] = ai;
                int c = count_triples_in_matrix(mat, ai);
                mat[i][j] = '.';

                if (c > 0)
                    return { i, j };
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (mat[i][j] == '.') {
                mat[i][j] = human;
                int c = count_triples_in_matrix(mat, human);
                mat[i][j] = '.';

                if (c > 0)
                    return { i, j };
            }
        }
    }

    if (mat[2][2] == '.')
        return { 2, 2 };

    vector<pair<int, int>> corners = { {0, 0}, {0, 4}, {4, 0}, {4, 4} };
    for (auto& p : corners)
        if (is_valid(p.first, p.second))
            return p;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (mat[i][j] == '.')
                return { i, j };

    return { 0, 0 };
}

Move<char>* Five_TTT_UI::get_move(Player<char>* player)
{
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "Enter move row col (0..4): ";
        cin >> x >> y;
    }
    else {
        Board<char>* b = player->get_board_ptr();
        char ai = player->get_symbol();
        char human = (ai == 'X' ? 'O' : 'X');

        auto best = find_best_ai_move(b, ai, human);
        x = best.first;
        y = best.second;

        cout << "[AI plays at (" << x << "," << y << ")]\n";
    }

    return new Move<char>(x, y, player->get_symbol());
}