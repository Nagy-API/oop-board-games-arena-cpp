#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib>

#include "XO_Classes.h"
#include "NumericalTicTacToe.h"
#include "BoardGame_Classes.h"
#include "SUS.h"
#include "5x5 Tic Tac Toe.h"
#include "Misere_Board.h"
#include "Infinity_XO.h"
#include "T4x4_Classes.h"
#include "Memory_XO.h"
#include "FourInRow.h"
#include "Word_Tic_Tac_Toe.h"
#include "Ultimate_TTT.h"
#include "pyramid.h"
#include "Diamond.h"
#include "Obstacle.h"

using namespace std;

enum class MatchResult {
    PLAYER1_WIN,
    PLAYER2_WIN,
    DRAW,
    UNKNOWN
};

struct TournamentScore {
    string player1Name = "Player 1";
    string player2Name = "Player 2";

    PlayerType player1Type = PlayerType::HUMAN;
    PlayerType player2Type = PlayerType::HUMAN;

    int player1Score = 0;
    int player2Score = 0;
    int draws = 0;
    int gamesPlayed = 0;
};

string player_type_to_string(PlayerType type) {
    return type == PlayerType::COMPUTER ? "Computer" : "Human";
}

void clear_score(TournamentScore& score) {
    score.player1Score = 0;
    score.player2Score = 0;
    score.draws = 0;
    score.gamesPlayed = 0;
}

void pause_screen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int read_int_in_range(const string& prompt, int minValue, int maxValue) {
    int value;

    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail() && value >= minValue && value <= maxValue) {
            return value;
        }

        cout << "Invalid choice. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string read_name(const string& prompt) {
    string name;

    while (true) {
        cout << prompt;
        getline(cin >> ws, name);

        if (!name.empty()) {
            return name;
        }

        cout << "Name cannot be empty. Try again.\n";
    }
}

PlayerType read_player_type(const string& playerLabel) {
    cout << "\nChoose " << playerLabel << " type:\n";
    cout << "1. Human\n";
    cout << "2. Computer\n";

    int choice = read_int_in_range("Choice: ", 1, 2);

    return choice == 2 ? PlayerType::COMPUTER : PlayerType::HUMAN;
}

void configure_players(TournamentScore& score) {
    cout << "\n====================================\n";
    cout << "          PLAYER SETUP\n";
    cout << "====================================\n";

    score.player1Name = read_name("Enter Player 1 name: ");
    score.player1Type = read_player_type(score.player1Name);

    cout << "\n";

    score.player2Name = read_name("Enter Player 2 name: ");
    score.player2Type = read_player_type(score.player2Name);

    clear_score(score);

    cout << "\nPlayers configured successfully.\n";
}

void show_score(const TournamentScore& score) {
    cout << "\n====================================\n";
    cout << "            SCOREBOARD\n";
    cout << "====================================\n";
    cout << score.player1Name << " (" << player_type_to_string(score.player1Type) << "): "
        << score.player1Score << "\n";

    cout << score.player2Name << " (" << player_type_to_string(score.player2Type) << "): "
        << score.player2Score << "\n";

    cout << "Draws: " << score.draws << "\n";
    cout << "Games Played: " << score.gamesPlayed << "\n";
    cout << "====================================\n";
}

void reset_score(TournamentScore& score) {
    clear_score(score);
    cout << "\nScore has been reset successfully.\n";
}

void update_score(TournamentScore& score, MatchResult result) {
    if (result == MatchResult::PLAYER1_WIN) {
        score.player1Score++;
        score.gamesPlayed++;
        cout << "\nResult recorded automatically: " << score.player1Name << " wins this game.\n";
    }
    else if (result == MatchResult::PLAYER2_WIN) {
        score.player2Score++;
        score.gamesPlayed++;
        cout << "\nResult recorded automatically: " << score.player2Name << " wins this game.\n";
    }
    else if (result == MatchResult::DRAW) {
        score.draws++;
        score.gamesPlayed++;
        cout << "\nResult recorded automatically: Draw.\n";
    }
    else {
        cout << "\nCould not detect the result automatically.\n";
        cout << "Score was not changed.\n";
    }
}

template <typename T>
MatchResult detect_result(Board<T>* board, Player<T>** players) {
    if (board == nullptr || players == nullptr || players[0] == nullptr || players[1] == nullptr) {
        return MatchResult::UNKNOWN;
    }

    if (board->is_win(players[0])) {
        return MatchResult::PLAYER1_WIN;
    }

    if (board->is_win(players[1])) {
        return MatchResult::PLAYER2_WIN;
    }

    if (board->is_lose(players[0])) {
        return MatchResult::PLAYER2_WIN;
    }

    if (board->is_lose(players[1])) {
        return MatchResult::PLAYER1_WIN;
    }

    if (board->is_draw(players[0]) || board->is_draw(players[1])) {
        return MatchResult::DRAW;
    }

    return MatchResult::UNKNOWN;
}

Player<char>** create_char_players(
    UI<char>* ui,
    const TournamentScore& score,
    char player1Symbol = 'X',
    char player2Symbol = 'O'
) {
    Player<char>** players = new Player<char>*[2];

    string name1 = score.player1Name;
    string name2 = score.player2Name;

    players[0] = ui->create_player(name1, player1Symbol, score.player1Type);
    players[1] = ui->create_player(name2, player2Symbol, score.player2Type);

    return players;
}

Player<int>** create_int_players(
    UI<int>* ui,
    const TournamentScore& score,
    int player1Symbol = 1,
    int player2Symbol = 2
) {
    Player<int>** players = new Player<int>*[2];

    string name1 = score.player1Name;
    string name2 = score.player2Name;

    players[0] = ui->create_player(name1, player1Symbol, score.player1Type);
    players[1] = ui->create_player(name2, player2Symbol, score.player2Type);

    return players;
}

template <typename T>
void cleanup_game(Board<T>* board, UI<T>* ui, Player<T>** players) {
    delete board;
    delete ui;

    if (players != nullptr) {
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
}

template <typename BoardType, typename UIType>
MatchResult run_standard_char_game(const TournamentScore& score) {
    UI<char>* ui = new UIType();
    Board<char>* board = new BoardType();
    Player<char>** players = create_char_players(ui, score, 'X', 'O');

    GameManager<char> game(board, players, ui);
    game.run();

    MatchResult result = detect_result(board, players);

    cleanup_game(board, ui, players);

    return result;
}

template <typename BoardType, typename UIType>
MatchResult run_standard_int_game(const TournamentScore& score) {
    UI<int>* ui = new UIType();
    Board<int>* board = new BoardType();
    Player<int>** players = create_int_players(ui, score, 1, 2);

    GameManager<int> game(board, players, ui);
    game.run();

    MatchResult result = detect_result(board, players);

    cleanup_game(board, ui, players);

    return result;
}

MatchResult run_sus_game_with_result(const TournamentScore& score) {
    UI<char>* ui = new SUS_UI();
    Board<char>* board = new SUS_Board();

    // SUS uses S and U, so Player 1 gets S and Player 2 gets U.
    Player<char>** players = create_char_players(ui, score, 'S', 'U');

    run_sus_game(board, players, ui);

    MatchResult result = detect_result(board, players);

    cleanup_game(board, ui, players);

    return result;
}

MatchResult run_pyramid_game_with_result(const TournamentScore& score) {
    PyramidBoard* board = new PyramidBoard();
    PyramidUI* ui = new PyramidUI(board);

    Player<char>** players = create_char_players(ui, score, 'X', 'O');

    GameManager<char> game(board, players, ui);
    game.run();

    MatchResult result = detect_result<char>(board, players);

    delete board;
    delete ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    return result;
}

MatchResult run_ultimate_game_with_result(const TournamentScore& score) {
    UltimateTTT_UI* ui = new UltimateTTT_UI();
    UltimateTTT_Board* board = new UltimateTTT_Board();

    Player<char>** players = create_char_players(ui, score, 'X', 'O');

    UltimateTTT_Manager mgr(board, players, ui);
    mgr.run();

    MatchResult result = detect_result<char>(board, players);

    delete board;
    delete ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    return result;
}

void show_games_menu() {
    cout << "\n====================================\n";
    cout << "          CHOOSE A GAME\n";
    cout << "====================================\n";
    cout << "1.  Play X-O (Tic-Tac-Toe)\n";
    cout << "2.  Play Numerical Tic-Tac-Toe\n";
    cout << "3.  Play SUS\n";
    cout << "4.  Play 5x5 Tic Tac Toe\n";
    cout << "5.  Play Misere\n";
    cout << "6.  Play Four-in-a-Row\n";
    cout << "7.  Play 4x4 Tic-Tac-Toe\n";
    cout << "8.  Play Word Tic-Tac-Toe\n";
    cout << "9.  Play Pyramid Tic-Tac-Toe\n";
    cout << "10. Play Diamond\n";
    cout << "11. Play Infinity XO\n";
    cout << "12. Play Ultimate XO\n";
    cout << "13. Play Memory XO\n";
    cout << "14. Play Obstacle\n";
    cout << "0.  Back to main menu\n";
    cout << "====================================\n";
}

MatchResult play_selected_game(int choice, const TournamentScore& score) {
    switch (choice) {
    case 1:
        return run_standard_char_game<X_O_Board, XO_UI>(score);

    case 2:
        return run_standard_int_game<NumericalTTT_Board, NumericalTTT_UI>(score);

    case 3:
        return run_sus_game_with_result(score);

    case 4:
        return run_standard_char_game<Five_TTT_Board, Five_TTT_UI>(score);

    case 5:
        return run_standard_char_game<MISERE_Board, MISERE_UI>(score);

    case 6:
        return run_standard_char_game<FourInRow_Board, FourInRow_UI>(score);

    case 7:
        return run_standard_char_game<T4x4_Board, T4x4_UI>(score);

    case 8:
        return run_standard_char_game<Word_Tic_Tac_Toe_Board, Word_Tic_Tac_Toe_UI>(score);

    case 9:
        return run_pyramid_game_with_result(score);

    case 10:
        return run_standard_char_game<DIAMOND_Board, DIAMOND_UI>(score);

    case 11:
        return run_standard_char_game<Infinity_XO_Board, Infinity_XO_UI>(score);

    case 12:
        return run_ultimate_game_with_result(score);

    case 13:
        return run_standard_char_game<Memory_XO_Board, Memory_XO_UI>(score);

    case 14:
        return run_standard_char_game<Obstacle_Board, Obstacle_UI>(score);

    default:
        return MatchResult::UNKNOWN;
    }
}

void play_game_flow(TournamentScore& score) {
    while (true) {
        show_score(score);
        show_games_menu();

        int gameChoice = read_int_in_range("Choose game: ", 0, 14);

        if (gameChoice == 0) {
            return;
        }

        MatchResult result = play_selected_game(gameChoice, score);
        update_score(score, result);
        show_score(score);

        cout << "\nWhat would you like to do next?\n";
        cout << "1. Play another game\n";
        cout << "2. Back to main menu\n";
        cout << "0. Exit program\n";

        int next = read_int_in_range("Choice: ", 0, 2);

        if (next == 1) {
            continue;
        }
        else if (next == 2) {
            return;
        }
        else {
            cout << "\nFinal score:\n";
            show_score(score);
            cout << "Goodbye!\n";
            exit(0);
        }
    }
}

void show_main_menu() {
    cout << "\n====================================\n";
    cout << "        BOARD GAMES ARENA\n";
    cout << "====================================\n";
    cout << "1. Play / Choose Game\n";
    cout << "2. Show Scoreboard\n";
    cout << "3. Reset Score\n";
    cout << "4. Change Players / Types\n";
    cout << "0. Exit\n";
    cout << "====================================\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    TournamentScore score;

    cout << "====================================\n";
    cout << "        BOARD GAMES ARENA\n";
    cout << "====================================\n";

    configure_players(score);

    while (true) {
        show_score(score);
        show_main_menu();

        int mainChoice = read_int_in_range("Choose option: ", 0, 4);

        if (mainChoice == 1) {
            play_game_flow(score);
        }
        else if (mainChoice == 2) {
            show_score(score);
            pause_screen();
        }
        else if (mainChoice == 3) {
            reset_score(score);
            pause_screen();
        }
        else if (mainChoice == 4) {
            configure_players(score);
            pause_screen();
        }
        else if (mainChoice == 0) {
            cout << "\nFinal score:\n";
            show_score(score);
            cout << "Goodbye!\n";
            break;
        }
    }

    return 0;
}