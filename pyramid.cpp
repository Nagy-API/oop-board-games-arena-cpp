#include "pyramid.h"
#include <iostream>
#include <limits>

using namespace std;

// ======================= Pyramid Board =======================

PyramidBoard::PyramidBoard() : Board<char>(3, 5) {
    board.clear();

    // Internal board layout:
    // row 0 = bottom row  -> 5 cells
    // row 1 = middle row  -> 3 cells
    // row 2 = top row     -> 1 cell
    board.push_back(vector<char>{'.', '.', '.', '.', '.'});
    board.push_back(vector<char>{'.', '.', '.'});
    board.push_back(vector<char>{'.'});
}

int PyramidBoard::abs_col(int row, int col) const {
    static int offsets[] = { 0, 1, 2 };
    return col + offsets[row];
}

bool PyramidBoard::update_board(Move<char>* move) {
    PyramidMove* mv = dynamic_cast<PyramidMove*>(move);

    if (!mv)
        return false;

    int r = mv->get_x();
    int c = mv->get_y();
    char s = mv->get_symbol();

    if (r < 0 || r >= static_cast<int>(board.size())) {
        cout << "Invalid row! Try again.\n";
        return false;
    }

    if (c < 0 || c >= static_cast<int>(board[r].size())) {
        cout << "Invalid column! Try again.\n";
        return false;
    }

    if (board[r][c] != '.') {
        cout << "Cell already occupied! Choose another.\n";
        return false;
    }

    board[r][c] = s;
    return true;
}

// ======================= Display Board =======================

void PyramidUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty())
        return;

    cout << "\n================ PYRAMID BOARD ================\n";
    cout << "Displayed rows:\n";
    cout << "Row 0 = Top row\n";
    cout << "Row 1 = Middle row\n";
    cout << "Row 2 = Bottom row\n\n";

    cout << "Board:\n\n";

    int base_width = static_cast<int>(matrix[0].size());

    // Display from top to bottom:
    // internal row 2 -> displayed row 0
    // internal row 1 -> displayed row 1
    // internal row 0 -> displayed row 2
    for (int internalRow = static_cast<int>(matrix.size()) - 1; internalRow >= 0; internalRow--) {
        int displayedRow = static_cast<int>(matrix.size()) - 1 - internalRow;
        int spaces = (base_width - static_cast<int>(matrix[internalRow].size())) / 2;

        cout << "Row " << displayedRow << ": ";
        cout << string(spaces * 2, ' ');

        for (int col = 0; col < static_cast<int>(matrix[internalRow].size()); col++) {
            char cell = matrix[internalRow][col];

            if (cell == '.')
                cout << ". ";
            else
                cout << cell << " ";
        }

        cout << "\n";
    }

    cout << "\nColumn indexes:\n";
    cout << "Row 0: 0\n";
    cout << "Row 1: 0 1 2\n";
    cout << "Row 2: 0 1 2 3 4\n";
    cout << "================================================\n\n";
}

// ======================= Get Move =======================

Move<char>* PyramidUI::get_move(Player<char>* player) {
    PyramidBoard* boardPtr = static_cast<PyramidBoard*>(player->get_board_ptr());

    char sym = player->get_symbol();
    int r = 0;
    int c = 0;

    if (!player->is_human()) {
        auto matrix = boardPtr->get_board_matrix();
        int rows = boardPtr->get_rows();

        char ai = sym;
        char opp = (ai == 'X' ? 'O' : 'X');

        bool foundMove = false;

        // 1. Try to win immediately
        for (int i = 0; i < rows && !foundMove; i++) {
            for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
                if (matrix[i][j] == '.') {
                    if (boardPtr->would_win_if(i, j, ai)) {
                        r = i;
                        c = j;
                        foundMove = true;
                        break;
                    }
                }
            }
        }

        // 2. Block opponent's immediate win
        for (int i = 0; i < rows && !foundMove; i++) {
            for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
                if (matrix[i][j] == '.') {
                    if (boardPtr->would_win_if(i, j, opp)) {
                        r = i;
                        c = j;
                        foundMove = true;
                        break;
                    }
                }
            }
        }

        // 3. Otherwise choose the first empty cell
        for (int i = 0; i < rows && !foundMove; i++) {
            for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
                if (matrix[i][j] == '.') {
                    r = i;
                    c = j;
                    foundMove = true;
                    break;
                }
            }
        }

        int displayedRow = (boardPtr->get_rows() - 1) - r;

        cout << player->get_name()
            << " (Computer) chooses row "
            << displayedRow
            << ", column "
            << c
            << "\n";
    }
    else {
        while (true) {
            int displayedRow;

            cout << player->get_name()
                << " ("
                << sym
                << ") enter displayed row and column: ";

            cin >> displayedRow >> c;

            if (cin.fail()) {
                cout << "Invalid input. Please enter two numbers, example: 2 4\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (displayedRow < 0 || displayedRow > 2) {
                cout << "Invalid row. Use row 0, 1, or 2.\n";
                continue;
            }

            // Convert displayed row to internal row
            r = (boardPtr->get_rows() - 1) - displayedRow;

            auto matrix = boardPtr->get_board_matrix();

            if (c < 0 || c >= static_cast<int>(matrix[r].size())) {
                cout << "Invalid column for this row.\n";

                if (displayedRow == 0)
                    cout << "Row 0 only has column: 0\n";
                else if (displayedRow == 1)
                    cout << "Row 1 has columns: 0 1 2\n";
                else
                    cout << "Row 2 has columns: 0 1 2 3 4\n";

                continue;
            }

            break;
        }
    }

    return new PyramidMove(r, c, sym);
}

// ======================= Win Checking =======================

bool PyramidBoard::is_win(Player<char>* player) {
    char s = player->get_symbol();
    int rows = static_cast<int>(board.size());

    // Horizontal lines
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j + 2 < static_cast<int>(board[i].size()); j++) {
            if (board[i][j] == s &&
                board[i][j + 1] == s &&
                board[i][j + 2] == s) {
                return true;
            }
        }
    }

    // Vertical-like lines using absolute column positions
    for (int r = 0; r + 2 < rows; r++) {
        for (int c = 0; c < static_cast<int>(board[r].size()); c++) {
            int a0 = abs_col(r, c);

            for (int c1 = 0; c1 < static_cast<int>(board[r + 1].size()); c1++) {
                if (abs_col(r + 1, c1) != a0)
                    continue;

                for (int c2 = 0; c2 < static_cast<int>(board[r + 2].size()); c2++) {
                    if (abs_col(r + 2, c2) != a0)
                        continue;

                    if (board[r][c] == s &&
                        board[r + 1][c1] == s &&
                        board[r + 2][c2] == s) {
                        return true;
                    }
                }
            }
        }
    }

    // Diagonal-like lines using absolute column positions
    for (int r = 0; r + 2 < rows; r++) {
        for (int c = 0; c < static_cast<int>(board[r].size()); c++) {
            int a0 = abs_col(r, c);

            // Diagonal right
            for (int c1 = 0; c1 < static_cast<int>(board[r + 1].size()); c1++) {
                if (abs_col(r + 1, c1) != a0 + 1)
                    continue;

                for (int c2 = 0; c2 < static_cast<int>(board[r + 2].size()); c2++) {
                    if (abs_col(r + 2, c2) != a0 + 2)
                        continue;

                    if (board[r][c] == s &&
                        board[r + 1][c1] == s &&
                        board[r + 2][c2] == s) {
                        return true;
                    }
                }
            }

            // Diagonal left
            for (int c1 = 0; c1 < static_cast<int>(board[r + 1].size()); c1++) {
                if (abs_col(r + 1, c1) != a0 - 1)
                    continue;

                for (int c2 = 0; c2 < static_cast<int>(board[r + 2].size()); c2++) {
                    if (abs_col(r + 2, c2) != a0 - 2)
                        continue;

                    if (board[r][c] == s &&
                        board[r + 1][c1] == s &&
                        board[r + 2][c2] == s) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool PyramidBoard::would_win_if(int row, int col, char sym) {
    if (row < 0 || row >= static_cast<int>(board.size()))
        return false;

    if (col < 0 || col >= static_cast<int>(board[row].size()))
        return false;

    if (board[row][col] != '.')
        return false;

    board[row][col] = sym;

    Player<char> tempPlayer("temp", sym, PlayerType::HUMAN);
    tempPlayer.set_board_ptr(this);

    bool result = is_win(&tempPlayer);

    board[row][col] = '.';

    return result;
}

bool PyramidBoard::is_lose(Player<char>* player) {
    return false;
}

bool PyramidBoard::is_draw(Player<char>* player) {
    for (auto& row : board) {
        for (char c : row) {
            if (c == '.')
                return false;
        }
    }

    return !is_win(player);
}

bool PyramidBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}