// NumericalTicTacToe.cpp
#include "NumericalTicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ==================== Board Implementation ====================

NumericalTTT_Board::NumericalTTT_Board() : Board(3, 3) {
    // Initialize board with zeros (empty)
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

// helper function: check if 3 numbers sum to 15
bool NumericalTTT_Board::three_sum_is_15(int a, int b, int c) const {
    return (a != blank_symbol && b != blank_symbol && c != blank_symbol && (a + b + c == 15));
}

bool NumericalTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int value = move->get_symbol();

    // check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // check if already filled or value used
    if (board[x][y] != blank_symbol || used_numbers.count(value))
        return false;

    // mark move
    board[x][y] = value;
    used_numbers.insert(value);
    n_moves++;
    return true;
}

bool NumericalTTT_Board::is_win(Player<int>* player) {
    // check all rows, cols, diagonals sum == 15
    for (int i = 0; i < 3; ++i) {
        if (three_sum_is_15(board[i][0], board[i][1], board[i][2])) return true;
        if (three_sum_is_15(board[0][i], board[1][i], board[2][i])) return true;
    }
    if (three_sum_is_15(board[0][0], board[1][1], board[2][2])) return true;
    if (three_sum_is_15(board[0][2], board[1][1], board[2][0])) return true;

    return false;
}

bool NumericalTTT_Board::is_lose(Player<int>* player) {
    return false; // not applicable
}

bool NumericalTTT_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool NumericalTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

// ==================== UI Implementation ====================

NumericalTTT_UI::NumericalTTT_UI() : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3) {}

Player<int>* NumericalTTT_UI::create_player(string& name, int symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (Player " << symbol << ")\n";
    return new Player<int>(name, symbol, type);
}

pair<int, int> NumericalTTT_UI::ask_coordinates() {
    int x, y;
    cout << "Enter cell position (x y): ";
    cin >> x >> y;
    return { x, y };
}

int NumericalTTT_UI::ask_number_from_player(Player<int>* player) {
    // نحدد الأرقام المسموح بها حسب اللاعب
    vector<int> valid_numbers;
    if (player->get_symbol() == 'X' || player->get_symbol() == 1)
        valid_numbers = { 1, 3, 5, 7, 9 }; // Player 1 → odd
    else
        valid_numbers = { 2, 4, 6, 8 };    // Player 2 → even

    // نجيب الأرقام المستخدمة على اللوحة
    Board<int>* boardPtr = player->get_board_ptr();
    set<int> used;
    for (auto& row : boardPtr->get_board_matrix())
        for (auto val : row)
            if (val != 0) used.insert(val);

    int num;
    while (true) {
        cout << "Enter a number to play (";
        for (int n : valid_numbers)
            if (!used.count(n)) cout << n << " ";
        cout << "): ";
        cin >> num;

        // لازم يكون رقم ضمن المسموح و مش مستخدم
        if (find(valid_numbers.begin(), valid_numbers.end(), num) != valid_numbers.end() && !used.count(num))
            return num;

        cout << "Invalid or already used number! Try again.\n";
    }
}

Move<int>* NumericalTTT_UI::get_move(Player<int>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        auto [x, y] = ask_coordinates();
        int value = ask_number_from_player(player);
        return new Move<int>(x, y, value);
    }
    else { // random computer move
        int x = rand() % 3;
        int y = rand() % 3;

        // odd numbers for player1, even for player2
        int value;
        if (player->get_symbol() == 'X' || player->get_symbol() == 1)
            value = 1 + 2 * (rand() % 5);   // odd: 1,3,5,7,9
        else
            value = 2 + 2 * (rand() % 4);   // even: 2,4,6,8

        return new Move<int>(x, y, value);
    }
}