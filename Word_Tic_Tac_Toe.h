#pragma once

#ifndef WORD_TTT_H
#define WORD_TTT_H

#include "BoardGame_Classes.h"
#include <set>
#include <string>

class Word_Tic_Tac_Toe_Board : public Board<char>
{
private:
    char blank = '.';
    std::set<std::string> dictionary;

    bool loadDictionary(const std::string& filename);
    bool checkWord(const std::string& word) const;

public:
    Word_Tic_Tac_Toe_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    bool isWinningMove(int x, int y, char letter) const;
};

class Word_Tic_Tac_Toe_UI : public UI<char>
{
public:
    Word_Tic_Tac_Toe_UI();

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif