#include "Word_Tic_Tac_Toe.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <cstdlib>

using namespace std;

Word_Tic_Tac_Toe_Board::Word_Tic_Tac_Toe_Board() : Board(3, 3)
{
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank;

    loadDictionary("dic.txt");
}

bool Word_Tic_Tac_Toe_Board::loadDictionary(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file) {
        std::cout << "Warning: Could not open dictionary file: " << filename << "\n";
        return false;
    }

    std::string word;

    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char ch) {
                return static_cast<char>(std::toupper(ch));
            });

        if (word.length() == 3) {
            dictionary.insert(word);
        }
    }

    return true;
}

bool Word_Tic_Tac_Toe_Board::checkWord(const std::string& word) const
{
    return dictionary.count(word) > 0;
}

bool Word_Tic_Tac_Toe_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board[x][y] != blank)
        return false;

    if (!std::isalpha(static_cast<unsigned char>(letter)))
        return false;

    board[x][y] = static_cast<char>(std::toupper(static_cast<unsigned char>(letter)));
    n_moves++;

    return true;
}

bool Word_Tic_Tac_Toe_Board::is_win(Player<char>* player)
{
    vector<string> words;

    // Rows
    for (int i = 0; i < 3; i++) {
        string word;
        word.push_back(board[i][0]);
        word.push_back(board[i][1]);
        word.push_back(board[i][2]);
        words.push_back(word);
    }

    // Columns
    for (int j = 0; j < 3; j++) {
        string word;
        word.push_back(board[0][j]);
        word.push_back(board[1][j]);
        word.push_back(board[2][j]);
        words.push_back(word);
    }

    // Diagonals
    string diagonal1;
    diagonal1.push_back(board[0][0]);
    diagonal1.push_back(board[1][1]);
    diagonal1.push_back(board[2][2]);
    words.push_back(diagonal1);

    string diagonal2;
    diagonal2.push_back(board[0][2]);
    diagonal2.push_back(board[1][1]);
    diagonal2.push_back(board[2][0]);
    words.push_back(diagonal2);

    for (const auto& word : words) {
        if (word.find(blank) == string::npos && checkWord(word)) {
            return true;
        }
    }

    return false;
}

bool Word_Tic_Tac_Toe_Board::is_draw(Player<char>* player)
{
    return n_moves == 9 && !is_win(player);
}

bool Word_Tic_Tac_Toe_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}

bool Word_Tic_Tac_Toe_Board::isWinningMove(int x, int y, char letter) const
{
    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board[x][y] != blank)
        return false;

    if (!std::isalpha(static_cast<unsigned char>(letter)))
        return false;

    letter = static_cast<char>(std::toupper(static_cast<unsigned char>(letter)));

    auto checkLine = [&](int r1, int c1, int r2, int c2, int r3, int c3) -> bool {
        char a = (r1 == x && c1 == y) ? letter : board[r1][c1];
        char b = (r2 == x && c2 == y) ? letter : board[r2][c2];
        char c = (r3 == x && c3 == y) ? letter : board[r3][c3];

        if (a == blank || b == blank || c == blank)
            return false;

        string word;
        word.push_back(a);
        word.push_back(b);
        word.push_back(c);

        return dictionary.count(word) > 0;
        };

    // Rows
    for (int i = 0; i < 3; i++) {
        if (checkLine(i, 0, i, 1, i, 2))
            return true;
    }

    // Columns
    for (int j = 0; j < 3; j++) {
        if (checkLine(0, j, 1, j, 2, j))
            return true;
    }

    // Diagonals
    if (checkLine(0, 0, 1, 1, 2, 2))
        return true;

    if (checkLine(0, 2, 1, 1, 2, 0))
        return true;

    return false;
}

Word_Tic_Tac_Toe_UI::Word_Tic_Tac_Toe_UI()
    : UI<char>("Welcome to Word Tic-Tac-Toe", 3)
{
}

Player<char>* Word_Tic_Tac_Toe_UI::create_player(std::string& name, char symbol, PlayerType type)
{
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << "\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Word_Tic_Tac_Toe_UI::get_move(Player<char>* player)
{
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        char letter;

        while (true) {
            cout << "\nEnter row col letter, example: 0 0 C : ";
            cin >> x >> y >> letter;

            if (cin.fail()) {
                cout << "Invalid input. Please enter: row column letter.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (x < 0 || x >= 3 || y < 0 || y >= 3) {
                cout << "Invalid position. Row and column must be from 0 to 2.\n";
                continue;
            }

            if (!isalpha(static_cast<unsigned char>(letter))) {
                cout << "Invalid letter. You must enter A-Z, not a number or symbol.\n";
                continue;
            }

            letter = static_cast<char>(toupper(static_cast<unsigned char>(letter)));
            return new Move<char>(x, y, letter);
        }
    }

    // Computer / AI move
    Word_Tic_Tac_Toe_Board* boardPtr =
        dynamic_cast<Word_Tic_Tac_Toe_Board*>(player->get_board_ptr());

    if (boardPtr == nullptr) {
        return new Move<char>(0, 0, 'A');
    }

    auto matrix = boardPtr->get_board_matrix();

    vector<pair<int, int>> emptyCells;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] == '.') {
                emptyCells.push_back({ i, j });
            }
        }
    }

    if (emptyCells.empty()) {
        return new Move<char>(0, 0, 'A');
    }

    // 1. Try to win immediately with any valid letter
    for (auto& cell : emptyCells) {
        int x = cell.first;
        int y = cell.second;

        for (char letter = 'A'; letter <= 'Z'; letter++) {
            if (boardPtr->isWinningMove(x, y, letter)) {
                cout << "[Computer plays at (" << x << "," << y << ") with letter " << letter << "]\n";
                return new Move<char>(x, y, letter);
            }
        }
    }

    // 2. Prefer center
    if (matrix[1][1] == '.') {
        char letter = static_cast<char>('A' + (rand() % 26));
        cout << "[Computer plays at (1,1) with letter " << letter << "]\n";
        return new Move<char>(1, 1, letter);
    }

    // 3. Prefer corners
    vector<pair<int, int>> corners = {
        {0, 0}, {0, 2}, {2, 0}, {2, 2}
    };

    for (auto& corner : corners) {
        int x = corner.first;
        int y = corner.second;

        if (matrix[x][y] == '.') {
            char letter = static_cast<char>('A' + (rand() % 26));
            cout << "[Computer plays at (" << x << "," << y << ") with letter " << letter << "]\n";
            return new Move<char>(x, y, letter);
        }
    }

    // 4. Fallback random empty cell
    pair<int, int> cell = emptyCells[rand() % emptyCells.size()];
    char letter = static_cast<char>('A' + (rand() % 26));

    cout << "[Computer plays at (" << cell.first << "," << cell.second
        << ") with letter " << letter << "]\n";

    return new Move<char>(cell.first, cell.second, letter);
}