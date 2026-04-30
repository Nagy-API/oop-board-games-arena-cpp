
#ifndef MISERE_CLASSES_H
#define MISERE_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

 
class MISERE_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    
    MISERE_Board();

    
    bool update_board(Move<char>* move);
 
    bool is_win(Player<char>* player);

    
    bool is_lose(Player<char>*) ;

   
    bool is_draw(Player<char>* player);

     
    bool game_is_over(Player<char>* player);
};

 
class MISERE_UI : public UI<char> {
public:
    
    MISERE_UI();
 
    ~MISERE_UI() {};
 
    Player<char>* create_player(string& name, char symbol, PlayerType type);
 
    virtual Move<char>* get_move(Player<char>* player);
};

#endif // MISERE_CLASSES_H
#pragma once
