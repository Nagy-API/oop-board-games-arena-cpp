// BoardGame_classes.h
// Version 2.3
// Date: 25 Nov 2023
// Author: Mohammad El-Ramly
// Putpose: Core classes to build board XO-style games (x-o, connect4, etc)

#ifndef _BOARDGAME_CLASSES_H
#define _BOARDGAME_CLASSES_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

/////////////////////////////////////////////////////////////
// Forward declarations
/////////////////////////////////////////////////////////////

template <typename T> class Player;
template <typename T> class Move;

/////////////////////////////////////////////////////////////
// Class declarations
/////////////////////////////////////////////////////////////

/**
 * @brief Represents the type of player in the game.
 */
enum class PlayerType {
    HUMAN,     ///< A human player.
    COMPUTER,  ///< A computer-controlled player.
    AI,        ///< An AI player.
    RANDOM     ///< A Random player.
};

/**
 * @brief Base template for any board used in board games.
 *
 * @tparam T Type of the elements stored on the board (e.g., char, int, string).
 *
 * Provides core data (rows, columns, matrix) and virtual methods to be
 * implemented by specific games like Tic-Tac-Toe, Connect4, etc.
 */
template <typename T>
class Board {
protected:
    int rows;        ///< Number of rows
    int columns;     ///< Number of columns
    vector<vector<T>> board; ///< 2D vector for the board
    int n_moves = 0; ///< Number of moves made

public:
    /**
     * @brief Construct a board with given dimensions.
     */
    Board(int rows, int columns)
        : rows(rows), columns(columns), board(rows, vector<T>(columns)) {
    }

    /**
     * @brief Virtual destructor. Frees allocated board memory.
     */
    virtual ~Board() {}

    /**
     * @brief Update the board with a new move.
     * @param move The move object containing position and symbol.
     * @return true if the move is valid and applied, false otherwise.
     */
    virtual bool update_board(Move<T>* move) = 0;

    /** @brief Check if a player has won. */
    virtual bool is_win(Player<T>*) = 0;

    /** @brief Check if a player has lost. */
    virtual bool is_lose(Player<T>*) = 0;

    /** @brief Check if the game ended in a draw. */
    virtual bool is_draw(Player<T>*) = 0;

    /** @brief Check if the game is over. */
    virtual bool game_is_over(Player<T>*) = 0;

    /**
     * @brief Return a copy of the current board as a 2D vector.
     */
    vector<vector<T>> get_board_matrix() const {
        return board;
    }

    /** @brief Get number of rows. */
    int get_rows() const { return rows; }

    /** @brief Get number of columns. */
    int get_columns() const { return columns; }

    /** @brief Get number of moves. */
    int get_n_moves() const { return n_moves; }

    /** @brief Return content of cell x, y in current board. */
    T get_cell(int x, int y) {
        return board[x][y];
    }
};

//-----------------------------------------------------
/**
 * @brief Represents a single move in a board game.
 *
 * @tparam T Type of symbol placed on the board (e.g., char, int).
 */
template <typename T>
class Move {
    int x;      ///< Row index
    int y;      ///< Column index
    T symbol;   ///< Symbol used in the move

public:
    /** @brief Construct a move at (x, y) using a symbol. */
    Move(int x, int y, T symbol) : x(x), y(y), symbol(symbol) {}
    virtual ~Move() {}

    /** @brief Get row index. */
    int get_x() const { return x; }

    /** @brief Get column index. */
    int get_y() const { return y; }

    /** @brief Get the move symbol. */
    T get_symbol() const { return symbol; }
};

//-----------------------------------------------------
/**
 * @brief Base template for all players (human or AI).
 *
 * @tparam T Type of symbol used by the player.
 */
template <typename T>
class Player {
protected:
    string name;         ///< Player name
    PlayerType type;     ///< Player type (e.g., HUMAN or COMPUTER)
    T symbol;            ///< Player’s symbol on board
    Board<T>* boardPtr;  ///< Pointer to the game board

public:
    /**
     * @brief Construct a player with name, symbol, and type.
     */
    Player(string n, T s, PlayerType t)
        : name(n), symbol(s), type(t), boardPtr(nullptr) {
    }

    virtual ~Player() {}
    virtual bool is_human() const { return type == PlayerType::HUMAN; }
    /** @brief Get the player's name. */
    string get_name() const { return name; }

    /** @brief Get player type (e.g., 'H' or 'C'). */
    PlayerType get_type() const { return type; }

    /** @brief Get the player's symbol. */
    T get_symbol() const { return symbol; }

    /** @brief Get a pointer to the game board. */
    Board<T>* get_board_ptr() const { return boardPtr; }

    /** @brief Assign the board pointer for the player. */
    void set_board_ptr(Board<T>* b) { boardPtr = b; }
};

//-----------------------------------------------------
/**
 * @brief Base class for handling user interface and input/output.
 *
 * @tparam T The type of symbol used on the board.
 */
template <typename T>
class UI {
protected:
    int cell_width; ///< Width of each displayed board cell

    /**
     * @brief Ask the user for the player's name.
     */
    virtual string get_player_name(string player_label) {
        string name;
        cout << "Enter " << player_label << " name: ";
        getline(cin >> ws, name);
        return name;
    }

    /**
     * @brief Ask the user to choose the player type from a list.
     */
    virtual PlayerType get_player_type_choice(string player_label, const vector<string>& options) {
        cout << "Choose " << player_label << " type:\n";
        for (size_t i = 0; i < options.size(); ++i)
            cout << i + 1 << ". " << options[i] << "\n";
        int choice;
        cin >> choice;
        return (choice == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;
    }

public:
    /**
     * @brief Construct the UI and display a welcome message.
     */
    UI(int cell_display_width = 3) : cell_width(cell_display_width) {}

    /**
     * @brief Construct the UI and display a welcome message.
     */
    UI(string message, int cell_display_width)
        : cell_width(cell_display_width) {
        cout << message << endl;
    }

    virtual ~UI() {}

    /** @brief Display any message to the user. */
    virtual void display_message(string message) { cout << message << "\n"; }

    /**
     * @brief Ask the user (or AI) to make a move.
     */
    virtual Move<T>* get_move(Player<T>*) = 0;

    /**
     * @brief Set up players for the game.
     */
    virtual Player<T>** setup_players();

    /**
     * @brief Create a player object based on input name, symbol, and type.
     */
    virtual Player<T>* create_player(string& name, T symbol, PlayerType type);

    /**
     * @brief Display the current board matrix in formatted form.
     */

    virtual void display_board_matrix(const vector<vector<T>>& matrix) const {
        if (matrix.empty() || matrix[0].empty()) return;

        int rows = matrix.size();
        int cols = matrix[0].size();

        cout << "\n    ";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width + 1) << j;
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

        for (int i = 0; i < rows; ++i) {
            cout << setw(2) << i << " |";
            for (int j = 0; j < cols; ++j)
                cout << setw(cell_width) << matrix[i][j] << " |";
            cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
        }
        cout << endl;
    }
};

//-----------------------------------------------------
/**
 * @brief Controls the flow of a board game between two players.
 *
 * @tparam T Type of symbol used on the board.
 */
template <typename T>
class GameManager {
    Board<T>* boardPtr;    ///< Game board
    Player<T>* players[2]; ///< Two players
    UI<T>* ui;             ///< User interface

public:
    /**
     * @brief Construct a game manager with board, players, and UI.
     */
    GameManager(Board<T>* b, Player<T>* p[2], UI<T>* u)
        : boardPtr(b), ui(u) {
        players[0] = p[0];
        players[1] = p[1];
        players[0]->set_board_ptr(b);
        players[1]->set_board_ptr(b);
    }

    /**
     * @brief Run the main game loop until someone wins or the game ends.
     */
    void run() {
        ui->display_board_matrix(boardPtr->get_board_matrix());
        Player<T>* currentPlayer = players[0];

        while (true) {
            for (int i : {0, 1}) {
                currentPlayer = players[i];
                Move<T>* move = ui->get_move(currentPlayer);

                while (!boardPtr->update_board(move))
                    move = ui->get_move(currentPlayer);

                ui->display_board_matrix(boardPtr->get_board_matrix());

                if (boardPtr->is_win(currentPlayer)) {
                    ui->display_message(currentPlayer->get_name() + " wins!");
                    return;
                }
                if (boardPtr->is_lose(currentPlayer)) {
                    ui->display_message(players[1 - i]->get_name() + " wins!");
                    return;
                }
                if (boardPtr->is_draw(currentPlayer)) {
                    ui->display_message("Draw!");
                    return;
                }
            }
        }
    }
};

//-----------------------------------------------------
/**
 * @brief Default implementation of setting up two players.
 */
template <typename T>
Player<T>** UI<T>::setup_players() {
    Player<T>** players = new Player<T>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    players[0] = create_player(nameX, static_cast<T>('X'), typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    players[1] = create_player(nameO, static_cast<T>('O'), typeO);

    return players;
}

/**
 * @brief Default implementation of creating two players.
 */
template <typename T>
Player<T>* UI<T>::create_player(string& name, T symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<T>(name, symbol, type);
}

#endif // _BOARDGAME_CLASSES_H
