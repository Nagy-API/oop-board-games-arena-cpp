#ifndef SUS_AI_H
#define SUS_AI_H

#include "SUS.h"
#include <vector>
#include <algorithm>
#include <climits>

class SUS_AI {
private:
    char ai_symbol;
    char opponent_symbol;

    int evaluate_move(const std::vector<std::vector<char>>& board, int x, int y, char symbol) const;
    int count_potential_SUS(const std::vector<std::vector<char>>& board, int x, int y, char symbol) const;

    bool forms_SUS(char a, char b, char c) const {
        return (a == 'S' && b == 'U' && c == 'S');
    }

    std::vector<std::pair<int, int>> get_empty_cells(const std::vector<std::vector<char>>& board) const;

public:
    SUS_AI(char symbol);
    Move<char>* get_best_move(Board<char>* board);
};

#endif