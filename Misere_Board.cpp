
#include "Misere_Board.h"

#include <iostream>
#include <iomanip>
#include <cctype>
#include "XO_Classes.h"

 
int minimax(MISERE_Board* board, char mySym, char oppSym, bool isMaximizing, int depth) {

    Player<char> pAI("ai", mySym, PlayerType::AI);
    Player<char> pHuman("human", oppSym, PlayerType::HUMAN);

    
    if (board->is_lose(&pAI))      
        return -10 + depth;

    if (board->is_lose(&pHuman))    
        return 10 - depth;

    if (board->is_draw(&pAI))       
        return 0;

    int bestScore = isMaximizing ? -100000 : 100000;

    for (int i = 0; i < board->get_rows(); i++) {

        for (int j = 0; j < board->get_columns(); j++) {

            if (board->get_board_matrix()[i][j] == '.') {

                // try move
                Move<char> mv(i, j, isMaximizing ? mySym : oppSym);
                board->update_board(&mv);

                int score = minimax(board, mySym, oppSym, !isMaximizing, depth + 1);

                // undo
                Move<char> undo(i, j, 0);
                board->update_board(&undo);

                if (isMaximizing)
                    bestScore = std::max(bestScore, score);
                else
                    bestScore = std::min(bestScore, score);
            }
        }
    }

    return bestScore;
}



 

MISERE_Board::MISERE_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool MISERE_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Valid square or undo
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {               // UNDO FIX
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}


bool MISERE_Board::is_win(Player<char>*)
{
    return false; // In Misère TicTacToe, nobody "wins"
}


 
 
bool MISERE_Board::is_lose(Player<char>* player)
{
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // rows & columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}


bool MISERE_Board::is_draw(Player<char>* player)
{
    return (n_moves == 9 && !is_lose(player));
}

bool MISERE_Board::game_is_over(Player<char>* player)
{
    return is_lose(player) || is_draw(player);
}


 

MISERE_UI::MISERE_UI() : UI<char>("Welcome to MISERE Game!", 3) {}

Player<char>* MISERE_UI::create_player(string& name, char symbol, PlayerType type)
{
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

 
Move<char>* MISERE_UI::get_move(Player<char>* player) {
    int x = 0, y = 0;

    if (player->get_type() == PlayerType::HUMAN) {

        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else {

        MISERE_Board* board = dynamic_cast<MISERE_Board*>(player->get_board_ptr());
        char mySym = player->get_symbol();
        char oppSym = (mySym == 'X') ? 'O' : 'X';

        int bestScore = -100000;

        for (int i = 0; i < board->get_rows(); i++) {
            for (int j = 0; j < board->get_columns(); j++) {

                if (board->get_board_matrix()[i][j] == '.') {

                    Move<char> mv(i, j, mySym);
                    board->update_board(&mv);

                    int score = minimax(board, mySym, oppSym, false, 0);

                    Move<char> undo(i, j, 0);
                    board->update_board(&undo);

                    if (score > bestScore) {
                        bestScore = score;
                        x = i;
                        y = j;
                    }
                }
            }
        }
    }

    return new Move<char>(x, y, player->get_symbol());
}
 

