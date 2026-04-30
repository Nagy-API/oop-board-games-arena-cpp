#ifndef T4X4_CLASSES_H
#define T4X4_CLASSES_H

#include "BoardGame_Classes.h"
#include <utility>
#include <limits>
#include <string>

using namespace std;

struct T4x4_Move {
    int fromRow, fromCol;
    int toRow, toCol;
};

class T4x4_MoveObj : public Move<char> {
private:
    int from_x, from_y;

public:
    T4x4_MoveObj(int fr, int fc, int tr, int tc, char sym)
        : Move<char>(tr, tc, sym), from_x(fr), from_y(fc) {
    }

    int get_from_x() const { return from_x; }
    int get_from_y() const { return from_y; }
};

class T4x4_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    T4x4_Board();

    bool IsValidMove(T4x4_Move move);
    bool update_board(Move<char>* move) override;
    void print_board();

    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    pair<int, int> find_movable_token(char sym, int toRow, int toCol);

    int evaluate_line(char a, char b, char c, char aiSym, char humanSym);
    int evaluate_board(char aiSym, char humanSym);
    int minimax(int depth, bool maximizing, char aiSym, char humanSym);
    T4x4_Move get_best_move(char aiSym, char humanSym);
};

class T4x4_UI : public UI<char> {
public:
    T4x4_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

private:
    void generate_computer_move(Player<char>* player, int& fr, int& fc, int& tr, int& tc);
};

#endif