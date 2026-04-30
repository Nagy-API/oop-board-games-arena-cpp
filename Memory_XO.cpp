#include "Memory_XO.h"
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

Memory_XO_Board::Memory_XO_Board()
    : Board<char>(3, 3)
{
    board_hidden.resize(3, vector<char>(3, blank_symbol));

    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Memory_XO_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = static_cast<char>(toupper(move->get_symbol()));

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board_hidden[x][y] == blank_symbol) {
        board_hidden[x][y] = mark;
        board[x][y] = blank_symbol;
        n_moves++;
        return true;
    }

    return false;
}

bool Memory_XO_Board::is_win(Player<char>* player)
{
    char sym = static_cast<char>(toupper(player->get_symbol()));

    auto all_equal = [&](char a, char b, char c) {
        return a == sym && b == sym && c == sym;
        };

    for (int i = 0; i < 3; i++) {
        if (all_equal(board_hidden[i][0], board_hidden[i][1], board_hidden[i][2]))
            return true;

        if (all_equal(board_hidden[0][i], board_hidden[1][i], board_hidden[2][i]))
            return true;
    }

    if (all_equal(board_hidden[0][0], board_hidden[1][1], board_hidden[2][2]))
        return true;

    if (all_equal(board_hidden[0][2], board_hidden[1][1], board_hidden[2][0]))
        return true;

    return false;
}

bool Memory_XO_Board::is_lose(Player<char>*)
{
    return false;
}

bool Memory_XO_Board::is_draw(Player<char>* player)
{
    return n_moves == 9 && !is_win(player);
}

bool Memory_XO_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}

Memory_XO_UI::Memory_XO_UI()
    : UI<char>("Welcome to Memory X-O Game", 3)
{
}

Player<char>* Memory_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Memory_XO_UI::get_move(Player<char>* player)
{
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move row and column (0-2): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }

    Memory_XO_Board* memoryBoard = dynamic_cast<Memory_XO_Board*>(player->get_board_ptr());

    if (memoryBoard != nullptr) {
        vector<vector<char>>& hidden = memoryBoard->get_hidden_board();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (hidden[i][j] == '.') {
                    cout << "[Computer plays at (" << i << "," << j << ")]\n";
                    return new Move<char>(i, j, player->get_symbol());
                }
            }
        }
    }

    x = rand() % 3;
    y = rand() % 3;

    cout << "[Computer plays at (" << x << "," << y << ")]\n";
    return new Move<char>(x, y, player->get_symbol());
}