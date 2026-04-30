#ifndef ULTIMATE_TICTACTOE_H
#define ULTIMATE_TICTACTOE_H

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include <vector>
#include <iostream>

using namespace std;

/*
 Ultimate Tic-Tac-Toe (simplified META version)
 - Big board: 3x3 where each cell is a small 3x3 X_O_Board
 - Players choose small board (1..9), then row (0..2) and col (0..2)
 - If a player wins a small board, bigBoard cell becomes that player's symbol
 - First player to have 3 in row in bigBoard wins
 - No forced sub-board rules (free choice)
*/

class UltimateTTT_Board : public Board<char> {
private:
    vector<vector<X_O_Board*>> smallBoards; // 3x3 of small XO boards
    vector<vector<char>> bigBoard;          // 3x3 summary board ('.' or 'X' or 'O' or 'T' tie)
    char blank = '.';

public:
    UltimateTTT_Board();
    virtual ~UltimateTTT_Board();

    // support two calling styles:
    //  - Manager will call play_in_subboard(boardNum, sr, sc, sym)
    //  - generic update_board accepts Move where x = boardNum (1..9), y = sr*10 + sc
    bool update_board(Move<char>* move) override;

    // direct helper used by manager:
    bool play_in_subboard(int boardNum1to9, int sr, int sc, char sym);

    void update_big_board(); // recompute bigBoard from smallBoards

    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>*) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // helpers for display / queries
    vector<vector<char>> get_big_board() const { return bigBoard; }
    X_O_Board* get_small_board(int br, int bc) { return smallBoards[br][bc]; }
    void display_full_board() const;
};


class UltimateTTT_UI : public UI<char> {
public:
    UltimateTTT_UI();
    ~UltimateTTT_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    // get_move returns a Move encoded as:
    // x = boardNum (1..9), y = sr*10 + sc
    // Manager will decode it; this lets UI be used with manager or generic GameManager
    Move<char>* get_move(Player<char>* player) override;
};


// custom manager because GameManager<T>::run assumes one-step moves only
class UltimateTTT_Manager {
    UltimateTTT_Board* boardPtr;
    Player<char>* players[2];
    UltimateTTT_UI* ui;
public:
    UltimateTTT_Manager(UltimateTTT_Board* b, Player<char>* p[2], UltimateTTT_UI* u);
    ~UltimateTTT_Manager();

    // run the game loop (handles two-step moves per turn)
    void run();
};

#endif // ULTIMATE_TICTACTOE_H