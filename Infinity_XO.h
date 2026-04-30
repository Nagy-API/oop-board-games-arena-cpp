#ifndef INFINITY_XO_CLASSES_H
#define INFINITY_XO_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>
using namespace std;

class Infinity_XO_Board : public Board<char> {
private:
    char blank_symbol = '.';
    deque<Move<char>> history;

public:
    Infinity_XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }

    bool game_is_over(Player<char>* player) override { return false; } 

private:
    void remove_oldest();  
public:
    void remove_oldest_after_win(); 
};

class Infinity_XO_UI : public UI<char> {
public:
    Infinity_XO_UI();
    ~Infinity_XO_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};


#endif // INFINITY_XO_CLASSES_H
