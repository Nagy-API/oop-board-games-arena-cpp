#pragma once

#ifndef FIVE_TTT_H
#define FIVE_TTT_H

#include "BoardGame_Classes.h"

class Five_TTT_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int countTriples(char s) const;

public:
    Five_TTT_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Five_TTT_UI : public UI<char> {
public:
    Five_TTT_UI();

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif