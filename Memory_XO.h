#pragma once

#ifndef MEMORY_XO_H
#define MEMORY_XO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Memory_XO_Board : public Board<char> {
private:
    char blank_symbol = '.';
    vector<vector<char>> board_hidden;

public:
    Memory_XO_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    vector<vector<char>>& get_hidden_board() {
        return board_hidden;
    }
};

class Memory_XO_UI : public UI<char> {
public:
    Memory_XO_UI();
    ~Memory_XO_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif