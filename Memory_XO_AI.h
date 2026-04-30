 #ifndef MEMORY_XO_AI_H
#define MEMORY_XO_AI_H

#include "Memory_XO.h"
#include <algorithm>

class Memory_XO_AI : public Player<char> {
private:
    char aiSym;
    char humanSym;

    int evaluate(Memory_XO_Board* board) {
        Player<char> ai("ai", aiSym, PlayerType::COMPUTER);
        Player<char> human("human", humanSym, PlayerType::HUMAN);

        if (board->is_win(&ai)) return 10;
        if (board->is_win(&human)) return -10;
        return 0;
    }

    int minimax(Memory_XO_Board* board, int depth, bool isMax) {
        int score = evaluate(board);
        if (score == 10 || score == -10) return score;
        if (depth == 9) return 0;

        if (isMax) {
            int best = -1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board->get_hidden_board()[i][j] == '.') {
                        board->get_hidden_board()[i][j] = aiSym;
                        best = std::max(best, minimax(board, depth + 1, false));
                        board->get_hidden_board()[i][j] = '.';
                    }
                }
            }
            return best;
        }
        else {
            int best = 1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board->get_hidden_board()[i][j] == '.') {
                        board->get_hidden_board()[i][j] = humanSym;
                        best = std::min(best, minimax(board, depth + 1, true));
                        board->get_hidden_board()[i][j] = '.';
                    }
                }
            }
            return best;
        }
    }

public:
    Memory_XO_AI(string name, char symbol)
        : Player<char>(name, symbol, PlayerType::COMPUTER) {
        aiSym = symbol;
        humanSym = (symbol == 'X') ? 'O' : 'X';
    }

    Move<char>* decide_move(Memory_XO_Board* board) {
        int bestVal = -1000;
        int bestX = -1, bestY = -1;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board->get_hidden_board()[i][j] == '.') {
                    board->get_hidden_board()[i][j] = aiSym;
                    int moveVal = minimax(board, 0, false);
                    board->get_hidden_board()[i][j] = '.';

                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        bestX = i;
                        bestY = j;
                    }
                }
            }
        }
        return new Move<char>(bestX, bestY, aiSym);
    }
};

#endif
