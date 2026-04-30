
#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include "Obstacle.h"

using namespace std;

Obstacle_Board::Obstacle_Board() : Board(6, 6) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}
bool Obstacle_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (board[x][y] != blank_symbol)
        return false;  

    // Apply move
    board[x][y] = toupper(mark);
    n_moves++;

    // After every 2 moves add obstacles
    if (n_moves % 2 == 0) {
        add_obstacles(2);
    }

    return true;
}

bool Obstacle_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    int directions[4][2] = {
        {1, 0}, 
        {0, 1},  
        {1, 1},  
        {1,-1}   
    };

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {

            if (board[r][c] != sym) continue;

            for (auto& d : directions) {
                int dr = d[0], dc = d[1];
                int count = 1;

                for (int k = 1; k < 4; k++) {
                    int nr = r + dr * k;
                    int nc = c + dc * k;

                    if (nr < 0 || nr >= rows || nc < 0 || nc >= columns)
                        break;

                    if (board[nr][nc] == sym)
                        count++;
                    else
                        break;
                }

                if (count == 4)
                    return true;
            }
        }
    }

    return false;
}
void Obstacle_Board::add_obstacles(int count) {
    vector<pair<int, int>> empty_cells;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++)
            if (board[r][c] == blank_symbol)
                empty_cells.emplace_back(r, c);

    if (empty_cells.empty()) return;

    for (int i = 0; i < count && !empty_cells.empty(); i++) {
        int idx = rand() % empty_cells.size();
        auto [r, c] = empty_cells[idx];
        board[r][c] = '#';
        empty_cells.erase(empty_cells.begin() + idx);
    }
}


bool Obstacle_Board::is_draw(Player<char>* player) {
    return (n_moves == rows * columns && !is_win(player));
}


bool Obstacle_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Obstacle_UI::Obstacle_UI() : UI<char>("Weclome to OBSTACLE!", 3) {}

Player<char>* Obstacle_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacle_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 5): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}