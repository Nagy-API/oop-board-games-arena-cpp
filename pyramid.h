#ifndef PYRAMID_H
#define PYRAMID_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
using namespace std;


class PyramidMove : public Move<char> {
public:
    PyramidMove(int x, int y, char s) : Move<char>(x, y, s) {}
};


class PyramidBoard : public Board<char> {
public:
    PyramidBoard();

    virtual bool update_board(Move<char>* move) override;
    virtual bool is_win(Player<char>* player) override;
    virtual bool is_lose(Player<char>* player) override;
    virtual bool is_draw(Player<char>* player) override;
    virtual bool game_is_over(Player<char>* player) override;
    bool would_win_if(int row, int col, char sym);

private:
 
    int abs_col(int row, int col) const;
};


class PyramidUI : public UI<char> {
    PyramidBoard* boardPtr;  
public:
   
    PyramidUI(PyramidBoard* board) : boardPtr(board), UI(3) {}

    
    virtual Move<char>* get_move(Player<char>* player) override;
    virtual void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // PYRAMID_H
