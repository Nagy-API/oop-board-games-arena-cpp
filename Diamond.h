#ifndef DIAMOND_BOARD_H
#define DIAMOND_BOARD_H

#include "BoardGame_Classes.h"
using namespace std;

class DIAMOND_Board : public Board<char> {
private:
    char blank_symbol = '.';
    char blocked_space = '#';

public:
    DIAMOND_Board();

    bool update_board(Move<char>* move);
    int count_line(int r, int c, int dx, int dy, char sym);

    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; }
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);

    int evaluate(char sym);
    int minimax(int depth, bool isMax, char aiSym, char humanSym);
    Move<char>* get_best_move(char aiSym, char humanSym);
};

class DIAMOND_UI : public UI<char> {
public:
    DIAMOND_UI();
    ~DIAMOND_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);
};

#endif
