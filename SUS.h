#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


void run_sus_game(Board<char>* board, Player<char>** players, UI<char>* ui);

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';

    int score_S = 0;
    int score_U = 0;

    int count_SUS_last_move(int x, int y) const;

public:
    SUS_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>*) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;

    int get_score_S() const { return score_S; }
    int get_score_U() const { return score_U; }
};


class SUS_UI : public UI<char> {
public:
    SUS_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    Player<char>** setup_players() override;

    Move<char>* get_move(Player<char>* player) override;

private:
    char ask_letter_choice();
};

#endif // SUS_CLASSES_H

