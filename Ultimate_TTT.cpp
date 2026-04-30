#include "Ultimate_TTT.h"
#include "Ultimate_TTT_AI.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ------------------ Board Implementation ------------------

UltimateTTT_Board::UltimateTTT_Board() : Board(3, 3), bigBoard(3, vector<char>(3, '.')) {
    smallBoards.resize(3, vector<X_O_Board*>(3, nullptr));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            smallBoards[i][j] = new X_O_Board();
    srand(static_cast<unsigned int>(time(0)));
}

UltimateTTT_Board::~UltimateTTT_Board() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            delete smallBoards[i][j];
}

bool UltimateTTT_Board::play_in_subboard(int boardNum1to9, int sr, int sc, char sym) {
    if (boardNum1to9 < 1 || boardNum1to9 > 9) return false;
    int idx = boardNum1to9 - 1;
    int br = idx / 3;
    int bc = idx % 3;
    if (sr < 0 || sr > 2 || sc < 0 || sc > 2) return false;

    if (bigBoard[br][bc] != '.') {
        return false;
    }

    X_O_Board* sb = smallBoards[br][bc];
    Move<char> m(sr, sc, sym);
    if (!sb->update_board(&m)) return false;
    n_moves++;
    update_big_board();
    return true;
}

bool UltimateTTT_Board::update_board(Move<char>* move) {
    int boardNum = move->get_x();
    int packed = move->get_y();
    if (boardNum < 1 || boardNum > 9) return false;
    int sr = packed / 10;
    int sc = packed % 10;
    char sym = move->get_symbol();
    return play_in_subboard(boardNum, sr, sc, sym);
}

void UltimateTTT_Board::update_big_board() {
    for (int bi = 0; bi < 3; ++bi) {
        for (int bj = 0; bj < 3; ++bj) {
            X_O_Board* sb = smallBoards[bi][bj];
            Player<char> tmpX("tmp", 'X', PlayerType::HUMAN); tmpX.set_board_ptr(sb);
            Player<char> tmpO("tmp", 'O', PlayerType::HUMAN); tmpO.set_board_ptr(sb);

            if (sb->is_win(&tmpX)) bigBoard[bi][bj] = 'X';
            else if (sb->is_win(&tmpO)) bigBoard[bi][bj] = 'O';
            else {
                auto mat = sb->get_board_matrix();
                bool full = true;
                for (auto& r : mat) for (auto& c : r) if (c == '.') full = false;
                if (full) bigBoard[bi][bj] = 'T';
                else bigBoard[bi][bj] = '.';
            }
        }
    }
}

bool UltimateTTT_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    for (int i = 0; i < 3; i++)
        if (bigBoard[i][0] == sym && bigBoard[i][1] == sym && bigBoard[i][2] == sym) return true;
    for (int j = 0; j < 3; j++)
        if (bigBoard[0][j] == sym && bigBoard[1][j] == sym && bigBoard[2][j] == sym) return true;
    if (bigBoard[0][0] == sym && bigBoard[1][1] == sym && bigBoard[2][2] == sym) return true;
    if (bigBoard[0][2] == sym && bigBoard[1][1] == sym && bigBoard[2][0] == sym) return true;
    return false;
}

bool UltimateTTT_Board::is_draw(Player<char>* player) {
    bool anyDot = false;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) if (bigBoard[i][j] == '.') anyDot = true;
    if (anyDot) return false;
    Player<char> tmpX("t", 'X', PlayerType::HUMAN); tmpX.set_board_ptr(nullptr);
    Player<char> tmpO("t", 'O', PlayerType::HUMAN); tmpO.set_board_ptr(nullptr);
    if (!is_win(&tmpX) && !is_win(&tmpO)) return true;
    return false;
}

bool UltimateTTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

void UltimateTTT_Board::display_full_board() const {
    cout << "\n================= ULTIMATE BOARD ==================\n";
    for (int br = 0; br < 3; ++br) {
        for (int r = 0; r < 3; ++r) {
            for (int bc = 0; bc < 3; ++bc) {
                auto mat = smallBoards[br][bc]->get_board_matrix();
                cout << ' ';
                cout << mat[r][0] << ' ' << mat[r][1] << ' ' << mat[r][2] << ' ';
                if (bc < 2) cout << " | ";
            }
            cout << "\n";
        }
        if (br < 2) {
            cout << "---------------------------------\n";
        }
    }

    cout << "\nBig board status:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) cout << bigBoard[i][j] << ' ';
        cout << "\n";
    }
    cout << "===================================================\n";
}


// ------------------ UI Implementation ------------------

UltimateTTT_UI::UltimateTTT_UI() : UI<char>("Welcome to Ultimate Tic Tac Toe (1..9 boards)", 3) {}

Player<char>* UltimateTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* UltimateTTT_UI::get_move(Player<char>* player) {
    // This is for human players only now
    int boardNum;
    int sr, sc;

    cout << player->get_name() << " (" << player->get_symbol() << ") - choose small board (1..9): ";
    cin >> boardNum;
    while (boardNum < 1 || boardNum > 9) {
        cout << "Invalid board number. Enter 1..9: ";
        cin >> boardNum;
    }

    cout << "Choose row (0..2): ";
    cin >> sr;
    cout << "Choose col (0..2): ";
    cin >> sc;

    int packed = sr * 10 + sc;
    return new Move<char>(boardNum, packed, player->get_symbol());
}


// ------------------ Manager Implementation ------------------

UltimateTTT_Manager::UltimateTTT_Manager(UltimateTTT_Board* b, Player<char>* p[2], UltimateTTT_UI* u) {
    boardPtr = b;
    players[0] = p[0];
    players[1] = p[1];
    players[0]->set_board_ptr(b);
    players[1]->set_board_ptr(b);
    ui = u;
}

UltimateTTT_Manager::~UltimateTTT_Manager() {}

void UltimateTTT_Manager::run() {
    int turn = 0;
    int forcedBoard = -1;

    UltimateTTT_AI* aiPlayers[2] = { nullptr, nullptr };

    for (int i = 0; i < 2; i++) {
        if (players[i]->get_type() == PlayerType::COMPUTER) {
            aiPlayers[i] = new UltimateTTT_AI(players[i]->get_symbol(), 3);
            cout << "AI Player created for " << players[i]->get_name()
                << " (" << players[i]->get_symbol() << ")\n";
        }
    }

    while (true) {
        boardPtr->display_full_board();
        Player<char>* cur = players[turn % 2];
        UltimateTTT_AI* curAI = aiPlayers[turn % 2];

        cout << "\nTurn: " << cur->get_name() << " (" << cur->get_symbol() << ")\n";

        int boardNum, sr, sc;

        if (cur->get_type() == PlayerType::COMPUTER && curAI != nullptr) {
            // AI PLAYER TURN
            cout << "AI is thinking...\n";

            auto bestMove = curAI->get_best_move(boardPtr, forcedBoard);
            boardNum = bestMove.first;
            sr = bestMove.second.first;
            sc = bestMove.second.second;

            cout << "[AI plays at board " << boardNum
                << ", position (" << sr << "," << sc << ")]\n";

        }
        else {
            // HUMAN PLAYER TURN
            if (forcedBoard == -1) {
                // Free selection
                Move<char>* mv = ui->get_move(cur);
                boardNum = mv->get_x();
                int packed = mv->get_y();
                sr = packed / 10;
                sc = packed % 10;
                delete mv;
            }
            else {
                // Forced board
                cout << "You must play in board " << forcedBoard << "\n";
                cout << "Row (0..2): ";
                cin >> sr;
                cout << "Col (0..2): ";
                cin >> sc;
                boardNum = forcedBoard;
            }
        }

        // Try move
        bool ok = boardPtr->play_in_subboard(boardNum, sr, sc, cur->get_symbol());
        if (!ok) {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        // Check if small board won/tied
        int idx = boardNum - 1;
        int br = idx / 3;
        int bc = idx % 3;
        char winner = boardPtr->get_big_board()[br][bc];

        if (winner == '.') {
            forcedBoard = boardNum;
        }
        else {
            if (winner == 'T') {
                forcedBoard = -1;
            }
            else {
                if (winner == cur->get_symbol()) {
                    forcedBoard = -1;
                }
                else {
                    turn++;
                    forcedBoard = -1;
                }
            }
        }

        // Check global win for both players
        Player<char>* p0 = players[0];
        Player<char>* p1 = players[1];

        if (boardPtr->is_win(p0)) {
            boardPtr->display_full_board();
            cout << "\n" << p0->get_name() << " wins the Ultimate game\n";

            for (int i = 0; i < 2; i++) {
                if (aiPlayers[i]) delete aiPlayers[i];
            }
            return;
        }

        if (boardPtr->is_win(p1)) {
            boardPtr->display_full_board();
            cout << "\n" << p1->get_name() << " wins the Ultimate game\n";

            for (int i = 0; i < 2; i++) {
                if (aiPlayers[i]) delete aiPlayers[i];
            }
            return;
        }

        // Check draw
        if (boardPtr->is_draw(p0)) {
            boardPtr->display_full_board();
            cout << "\nGame is a draw\n";

            for (int i = 0; i < 2; i++) {
                if (aiPlayers[i]) delete aiPlayers[i];
            }
            return;
        }

        turn++;
    }
}
