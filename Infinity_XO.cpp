#include "Infinity_XO.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
using namespace std;

//==================== Board ====================

Infinity_XO_Board::Infinity_XO_Board() : Board<char>(3, 3)
{
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Infinity_XO_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = toupper(move->get_symbol());

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (board[x][y] != blank_symbol)
        return false;

    board[x][y] = mark;
    n_moves++;
    history.push_back(*move);


    if (history.size() >= 3)
        remove_oldest();

    return true;
}

bool Infinity_XO_Board::is_win(Player<char>* player)
{
    char sym = player->get_symbol();

    auto eq = [&](char a, char b, char c) {
        return a == b && b == c && a == sym;
        };

    for (int i = 0; i < 3; i++) {
        if (eq(board[i][0], board[i][1], board[i][2])) return true;
        if (eq(board[0][i], board[1][i], board[2][i])) return true;
    }

    if (eq(board[0][0], board[1][1], board[2][2])) return true;
    if (eq(board[0][2], board[1][1], board[2][0])) return true;

    return false;
}

bool Infinity_XO_Board::is_draw(Player<char>* player)
{
    return (n_moves >= 9 && !is_win(player));
}

void Infinity_XO_Board::remove_oldest()
{
    if (n_moves > 3 && (n_moves - 3) % 3 == 0 && history.size() >= 3)
    {
        Move<char> oldest = history.front();
        history.pop_front();

        int x = oldest.get_x();
        int y = oldest.get_y();

        board[x][y] = blank_symbol;
        cout << "\n>> Removed oldest mark at (" << x << "," << y << ")\n";
    }
}

void Infinity_XO_Board::remove_oldest_after_win()
{
    if (!history.empty())
    {
        Move<char> oldest = history.front();
        history.pop_front();

        int x = oldest.get_x();
        int y = oldest.get_y();

        board[x][y] = blank_symbol;
        cout << "\n>> Removed oldest mark after win/draw at (" << x << "," << y << ")\n";
    }
}

//==================== UI ====================

Infinity_XO_UI::Infinity_XO_UI() : UI<char>("=== Infinity Tic-Tac-Toe ===", 3) {}

Player<char>* Infinity_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    cout << "Creating " << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Infinity_XO_UI::get_move(Player<char>* player)
{
    int x, y;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlayer " << player->get_name() << " (" << player->get_symbol() << ") enter x y (0-2): ";
        cin >> x >> y;
    }
    else
    {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }

    return new Move<char>(x, y, player->get_symbol());
}
