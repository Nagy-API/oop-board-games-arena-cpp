#include "SUS.h"
#include "SUS_AI.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
using namespace std;

//////////////////////// BOARD ////////////////////////

SUS_Board::SUS_Board() : Board(3, 3)
{
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

int SUS_Board::count_SUS_last_move(int x, int y) const
{
    int cnt = 0;

    auto is_SUS = [&](char a, char b, char c)
        {
            return (a == 'S' && b == 'U' && c == 'S');
        };

    if (is_SUS(board[x][0], board[x][1], board[x][2]))
        cnt++;

    if (is_SUS(board[0][y], board[1][y], board[2][y]))
        cnt++;

    if (x == y && is_SUS(board[0][0], board[1][1], board[2][2]))
        cnt++;

    if (x + y == 2 && is_SUS(board[0][2], board[1][1], board[2][0]))
        cnt++;

    return cnt;
}

bool SUS_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char sym = toupper(move->get_symbol());

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (board[x][y] != blank_symbol)
        return false;

    board[x][y] = sym;
    n_moves++;

    int gained = count_SUS_last_move(x, y);

    if (sym == 'S')
        score_S += gained;
    else if (sym == 'U')
        score_U += gained;

    return true;
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves < 9) return false;
    char sym = toupper(player->get_symbol());
    if (score_S > score_U && sym == 'S') return true;
    if (score_U > score_S && sym == 'U') return true;
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && score_S == score_U);
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return (n_moves == 9);
}

//////////////////////// UI ////////////////////////

SUS_UI::SUS_UI() : UI<char>("Welcome to SUS Game!", 3) {}

char SUS_UI::ask_letter_choice()
{
    char c;
    while (true) {
        cout << "Choose your letter (S or U): ";
        cin >> c;
        c = toupper(c);
        if (c == 'S' || c == 'U')
            return c;
        cout << "Invalid choice. Try again.\n";
    }
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Player<char>** SUS_UI::setup_players()
{
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1");
    PlayerType t1 = get_player_type_choice("Player 1", type_options);
    char sym1 = ask_letter_choice();
    players[0] = create_player(name1, sym1, t1);

    string name2 = get_player_name("Player 2");
    PlayerType t2 = get_player_type_choice("Player 2", type_options);
    char sym2 = (sym1 == 'S' ? 'U' : 'S');
    players[1] = create_player(name2, sym2, t2);

    return players;
}

void run_sus_game(Board<char>* board, Player<char>** players, UI<char>* ui) {
    ui->display_board_matrix(board->get_board_matrix());

    static SUS_AI* ai_player1 = nullptr;
    static SUS_AI* ai_player2 = nullptr;

    for (int turn = 0; turn < 9; ++turn) {
        int i = turn % 2;
        Player<char>* currentPlayer = players[i];

        Move<char>* move = nullptr;

        // ✅ لو كمبيوتر، استخدم الـ AI مباشرة هنا
        if (currentPlayer->get_type() == PlayerType::COMPUTER) {
            char sym = toupper(currentPlayer->get_symbol());

            if (sym == 'S') {
                if (!ai_player1) ai_player1 = new SUS_AI('S');
                move = ai_player1->get_best_move(board);
            }
            else {
                if (!ai_player2) ai_player2 = new SUS_AI('U');
                move = ai_player2->get_best_move(board);
            }
        }
        else {
            // لو human، استخدم الـ UI
            move = ui->get_move(currentPlayer);
        }

        while (!board->update_board(move)) {
            if (currentPlayer->get_type() == PlayerType::COMPUTER) {
                char sym = toupper(currentPlayer->get_symbol());
                if (sym == 'S') move = ai_player1->get_best_move(board);
                else move = ai_player2->get_best_move(board);
            }
            else {
                move = ui->get_move(currentPlayer);
            }
        }

        ui->display_board_matrix(board->get_board_matrix());

        if (board->game_is_over(currentPlayer)) {
            if (board->is_draw(currentPlayer))
                ui->display_message("Draw!");
            else if (board->is_win(players[0]))
                ui->display_message(players[0]->get_name() + " wins!");
            else if (board->is_win(players[1]))
                ui->display_message(players[1]->get_name() + " wins!");
            return;
        }
    }
}

Move<char>* SUS_UI::get_move(Player<char>* player)
{
    int x, y;
    cout << player->get_name() << " enter move (x y): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}
