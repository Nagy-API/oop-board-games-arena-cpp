#include <iostream>
#include <cctype>
#include <algorithm>
#include "Diamond.h"

using namespace std;

// ======================= Board Constructor =======================

DIAMOND_Board::DIAMOND_Board() : Board(7, 7)
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {

            bool allowed = false;

            if (i == 0 && j == 3) allowed = true;
            else if (i == 1 && j >= 2 && j <= 4) allowed = true;
            else if (i == 2 && j >= 1 && j <= 5) allowed = true;
            else if (i == 3) allowed = true;
            else if (i == 4 && j >= 1 && j <= 5) allowed = true;
            else if (i == 5 && j >= 2 && j <= 4) allowed = true;
            else if (i == 6 && j == 3) allowed = true;

            board[i][j] = allowed ? blank_symbol : blocked_space;
        }
    }
}

// ======================= Update Board =======================

bool DIAMOND_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= 7 || y < 0 || y >= 7)
        return false;

    if (board[x][y] == blocked_space)
        return false;

    if (board[x][y] != blank_symbol && mark != 0)
        return false;

    if (mark == 0) {
        board[x][y] = blank_symbol;
        n_moves--;
    }
    else {
        board[x][y] = static_cast<char>(toupper(static_cast<unsigned char>(mark)));
        n_moves++;
    }

    return true;
}

// ======================= Helpers =======================

int DIAMOND_Board::count_line(int r, int c, int dx, int dy, char sym)
{
    int total = 1;

    int rr = r + dx;
    int cc = c + dy;

    while (rr >= 0 && rr < 7 && cc >= 0 && cc < 7 && board[rr][cc] == sym) {
        total++;
        rr += dx;
        cc += dy;
    }

    rr = r - dx;
    cc = c - dy;

    while (rr >= 0 && rr < 7 && cc >= 0 && cc < 7 && board[rr][cc] == sym) {
        total++;
        rr -= dx;
        cc -= dy;
    }

    return total;
}

// ======================= Win / Draw =======================

bool DIAMOND_Board::is_win(Player<char>* player)
{
    char sym = player->get_symbol();

    int dx[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    int dy[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };

    bool found3 = false;
    bool found4 = false;

    int dir3 = -1;
    int dir4 = -1;

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != sym)
                continue;

            if (board[r][c] == blocked_space)
                continue;

            for (int d = 0; d < 8; d++) {
                int count = count_line(r, c, dx[d], dy[d], sym);

                if (count == 3 && !found3) {
                    found3 = true;
                    dir3 = d;
                }

                if (count == 4 && !found4) {
                    found4 = true;
                    dir4 = d;
                }

                if (found3 && found4 && dir3 != dir4) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool DIAMOND_Board::is_draw(Player<char>* player)
{
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if (board[r][c] == blank_symbol)
                return false;
        }
    }

    Player<char> p1("p1", 'X', PlayerType::HUMAN);
    Player<char> p2("p2", 'O', PlayerType::COMPUTER);

    return !is_win(&p1) && !is_win(&p2);
}

bool DIAMOND_Board::game_is_over(Player<char>* player)
{
    Player<char> p1("p1", 'X', PlayerType::HUMAN);
    Player<char> p2("p2", 'O', PlayerType::COMPUTER);

    if (is_win(&p1))
        return true;

    if (is_win(&p2))
        return true;

    return is_draw(player);
}

// ======================= Evaluation =======================

int DIAMOND_Board::evaluate(char sym)
{
    int score = 0;

    Player<char> temp("temp", sym, PlayerType::COMPUTER);

    if (is_win(&temp))
        return 100000;

    int dx[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    int dy[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != sym)
                continue;

            for (int d = 0; d < 8; d++) {
                int lineLength = count_line(r, c, dx[d], dy[d], sym);

                if (lineLength >= 4)
                    score += 500;

                else if (lineLength == 3)
                    score += 120;

                else if (lineLength == 2)
                    score += 25;
            }
        }
    }

    return score;
}

// موجودة عشان الـ header معلنها، لكن الـ AI السريع تحت مش بيعتمد عليها
// عشان نتجنب البطء الشديد في Diamond.
int DIAMOND_Board::minimax(int depth, bool isMax, char aiSym, char humanSym)
{
    Player<char> tempAI("AI", aiSym, PlayerType::COMPUTER);
    Player<char> tempHuman("Human", humanSym, PlayerType::HUMAN);

    if (is_win(&tempAI))
        return 10000 - depth;

    if (is_win(&tempHuman))
        return -10000 + depth;

    if (depth == 0)
        return evaluate(aiSym) - evaluate(humanSym);

    int best = isMax ? -999999 : 999999;

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != blank_symbol)
                continue;

            board[r][c] = isMax ? aiSym : humanSym;

            int value = minimax(depth - 1, !isMax, aiSym, humanSym);

            board[r][c] = blank_symbol;

            if (isMax)
                best = max(best, value);
            else
                best = min(best, value);
        }
    }

    return best;
}

// ======================= Fast AI =======================

Move<char>* DIAMOND_Board::get_best_move(char aiSym, char humanSym)
{
    aiSym = static_cast<char>(toupper(static_cast<unsigned char>(aiSym)));
    humanSym = static_cast<char>(toupper(static_cast<unsigned char>(humanSym)));

    // 1. لو الـ AI يقدر يكسب دلوقتي، يلعب حركة المكسب فورًا
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != blank_symbol)
                continue;

            board[r][c] = aiSym;

            Player<char> aiPlayer("AI", aiSym, PlayerType::COMPUTER);
            bool canWin = is_win(&aiPlayer);

            board[r][c] = blank_symbol;

            if (canWin) {
                cout << "[Computer found a winning move at (" << r << "," << c << ")]\n";
                return new Move<char>(r, c, aiSym);
            }
        }
    }

    // 2. لو اللاعب يقدر يكسب الحركة الجاية، الـ AI يمنعه
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != blank_symbol)
                continue;

            board[r][c] = humanSym;

            Player<char> humanPlayer("Human", humanSym, PlayerType::HUMAN);
            bool humanCanWin = is_win(&humanPlayer);

            board[r][c] = blank_symbol;

            if (humanCanWin) {
                cout << "[Computer blocks your winning move at (" << r << "," << c << ")]\n";
                return new Move<char>(r, c, aiSym);
            }
        }
    }

    // 3. تقييم سريع لكل خانة فاضية:
    //    - يقوي خطوط الـ AI
    //    - يقلل فرص اللاعب
    //    - يفضل المنتصف والخانات المهمة
    int bestScore = -999999;
    int bestX = -1;
    int bestY = -1;

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {

            if (board[r][c] != blank_symbol)
                continue;

            board[r][c] = aiSym;

            int aiScore = evaluate(aiSym);
            int humanScore = evaluate(humanSym);

            board[r][c] = blank_symbol;

            int centerBonus = 0;

            // تفضيل الخانات القريبة من منتصف الدايموند
            int distanceFromCenter = abs(r - 3) + abs(c - 3);

            if (distanceFromCenter == 0)
                centerBonus = 80;
            else if (distanceFromCenter == 1)
                centerBonus = 50;
            else if (distanceFromCenter == 2)
                centerBonus = 25;
            else
                centerBonus = 5;

            int totalScore = aiScore - humanScore + centerBonus;

            if (totalScore > bestScore) {
                bestScore = totalScore;
                bestX = r;
                bestY = c;
            }
        }
    }

    // 4. لو لسبب ما مفيش best move، اختار أول خانة فاضية
    if (bestX == -1 || bestY == -1) {
        for (int r = 0; r < 7; r++) {
            for (int c = 0; c < 7; c++) {
                if (board[r][c] == blank_symbol) {
                    bestX = r;
                    bestY = c;
                    break;
                }
            }

            if (bestX != -1)
                break;
        }
    }

    cout << "[Computer plays at (" << bestX << "," << bestY << ")]\n";
    return new Move<char>(bestX, bestY, aiSym);
}

// ======================= UI =======================

DIAMOND_UI::DIAMOND_UI()
    : UI<char>("Welcome to DIAMOND", 3)
{
}

Player<char>* DIAMOND_UI::create_player(string& name, char symbol, PlayerType type)
{
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* DIAMOND_UI::get_move(Player<char>* player)
{
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;

        cout << "\nEnter x y (0..6): ";
        cin >> x >> y;

        return new Move<char>(x, y, player->get_symbol());
    }

    DIAMOND_Board* db = dynamic_cast<DIAMOND_Board*>(player->get_board_ptr());

    char aiSym = player->get_symbol();
    char humanSym = (aiSym == 'X' ? 'O' : 'X');

    return db->get_best_move(aiSym, humanSym);
}