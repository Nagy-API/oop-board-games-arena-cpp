
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "BoardGame_Classes.h"
using namespace std;
 
class Obstacle_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    char blocked_space = '#';
public:
    
    Obstacle_Board();
     
    bool update_board(Move<char>* move);
 
    bool is_win(Player<char>* player);
 
    bool is_lose(Player<char>*) { return false; };

    void  add_obstacles(int count);
 
    bool is_draw(Player<char>* player);
     
    bool game_is_over(Player<char>* player);
};
 
class Obstacle_UI : public UI<char> {
public:
     
    Obstacle_UI();
     
    ~Obstacle_UI() {};
 
    Player<char>* create_player(string& name, char symbol, PlayerType type);
 
    virtual Move<char>* get_move(Player<char>* player);
};

#endif // OBSTACLE_H#pragma once
#pragma once
