#include "T4x4_Classes.h"
#include <iostream>
#include <cmath>
using namespace std;

T4x4_Board::T4x4_Board() : Board(4, 4) {
	for (auto& row : board)
		for (auto& cell : row)
			cell = blank_symbol;

	board[0][0] = 'O'; board[0][1] = 'X'; board[0][2] = 'O'; board[0][3] = 'X';
	board[3][0] = 'X'; board[3][1] = 'O'; board[3][2] = 'X'; board[3][3] = 'O';
	n_moves = 8;

}

// ===== Valid move =====
bool T4x4_Board::IsValidMove(T4x4_Move mv) {
	if (mv.fromRow < 0 || mv.fromRow >= rows || mv.fromCol < 0 || mv.fromCol >= columns) return false;
	if (mv.toRow < 0 || mv.toRow >= rows || mv.toCol < 0 || mv.toCol >= columns) return false;
	if (board[mv.fromRow][mv.fromCol] == blank_symbol) return false;
	if (board[mv.toRow][mv.toCol] != blank_symbol) return false;
	int dR = abs(mv.fromRow - mv.toRow);
	int dC = abs(mv.fromCol - mv.toCol);
	return (dR + dC == 1);
}

pair<int, int> T4x4_Board::find_movable_token(char sym, int toRow, int toCol) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			if (board[i][j] == sym) {
				T4x4_Move mv{ i,j,toRow,toCol };
				if (IsValidMove(mv)) return { i,j };
			}
	return { -1,-1 };
}

// ===== Update board =====
bool T4x4_Board::update_board(Move<char>* move) {
	T4x4_MoveObj* mv = dynamic_cast<T4x4_MoveObj*>(move);
	if (!mv) return false;
	int fr = mv->get_from_x(), fc = mv->get_from_y();
	int toRow = mv->get_x(), toCol = mv->get_y();
	char sym = mv->get_symbol();

	if (board[fr][fc] != sym) { cout << "Invalid move!\n"; return false; }
	if (board[toRow][toCol] != '.') { cout << "Invalid move!\n"; return false; }
	int dR = abs(fr - toRow), dC = abs(fc - toCol);
	if (dR + dC != 1) { cout << "Invalid move!\n"; return false; }

	board[fr][fc] = '.';
	board[toRow][toCol] = sym;
	return true;

}

void T4x4_Board::print_board() {
	for (auto& row : board) {
		for (auto& c : row) cout << c << " ";
		cout << "\n";
	}
}

// ===== Win/Lose/Draw =====
bool T4x4_Board::is_win(Player<char>* player) {
	char s = player->get_symbol();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			if (board[i][j] == s && board[i][j + 1] == s && board[i][j + 2] == s) return true;
			if (board[j][i] == s && board[j + 1][i] == s && board[j + 2][i] == s) return true;
		}
	}
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			if (board[i][j] == s && board[i + 1][j + 1] == s && board[i + 2][j + 2] == s) return true;
	for (int i = 0; i < 2; i++)
		for (int j = 2; j < 4; j++)
			if (board[i][j] == s && board[i + 1][j - 1] == s && board[i + 2][j - 2] == s) return true;
	return false;
}
bool T4x4_Board::is_lose(Player<char>* player) { return is_win(player->get_symbol() == 'X' ? new Player<char>("AI", 'O', PlayerType::AI) : new Player<char>("AI", 'X', PlayerType::AI)); }
bool T4x4_Board::is_draw(Player<char>*) { return false; }
bool T4x4_Board::game_is_over(Player<char>* player) { return is_win(player); }

// ===== AI =====
int T4x4_Board::evaluate_line(char a, char b, char c, char aiSym, char humanSym) {
	int aiCount = (a == aiSym) + (b == aiSym) + (c == aiSym);
	int humanCount = (a == humanSym) + (b == humanSym) + (c == humanSym);
	if (aiCount == 3) return 100;
	if (humanCount == 3) return -100;
	if (aiCount == 2 && humanCount == 0) return 10;
	if (humanCount == 2 && aiCount == 0) return -10;
	return 0;
}

int T4x4_Board::evaluate_board(char aiSym, char humanSym) {
	int score = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 2; j++)
			score += evaluate_line(board[i][j], board[i][j + 1], board[i][j + 2], aiSym, humanSym);
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 2; i++)
			score += evaluate_line(board[i][j], board[i + 1][j], board[i + 2][j], aiSym, humanSym);
	score += evaluate_line(board[0][0], board[1][1], board[2][2], aiSym, humanSym);
	score += evaluate_line(board[0][3], board[1][2], board[2][1], aiSym, humanSym);
	return score;
}

int T4x4_Board::minimax(int depth, bool maximizing, char aiSym, char humanSym) {
	int score = evaluate_board(aiSym, humanSym);
	if (depth == 0 || abs(score) >= 100) return score;
	if (maximizing) {
		int best = -10000;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (board[i][j] == aiSym) {
					int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
					for (auto& d : dirs) {
						int ni = i + d[0], nj = j + d[1];
						if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && board[ni][nj] == '.') {
							board[i][j] = '.';
							board[ni][nj] = aiSym;
							best = max(best, minimax(depth - 1, false, aiSym, humanSym));
							board[i][j] = aiSym;
							board[ni][nj] = '.';
						}
					}
				}
			}
		}
		return best;
	}
	else {
		int best = 10000;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (board[i][j] == humanSym) {
					int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
					for (auto& d : dirs) {
						int ni = i + d[0], nj = j + d[1];
						if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && board[ni][nj] == '.') {
							board[i][j] = '.';
							board[ni][nj] = humanSym;
							best = min(best, minimax(depth - 1, true, aiSym, humanSym));
							board[i][j] = humanSym;
							board[ni][nj] = '.';
						}
					}
				}
			}
		}
		return best;
	}
}

T4x4_Move T4x4_Board::get_best_move(char aiSym, char humanSym) {
	int bestScore = -10000;
	T4x4_Move bestMove{ -1,-1,-1,-1 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == aiSym) {
				int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
				for (auto& d : dirs) {
					int ni = i + d[0], nj = j + d[1];
					if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && board[ni][nj] == '.') {
						board[i][j] = '.';
						board[ni][nj] = aiSym;
						int score = minimax(2, false, aiSym, humanSym); 
						if (score > bestScore) {
							bestScore = score;
							bestMove = { i,j,ni,nj };
						}
						board[i][j] = aiSym;
						board[ni][nj] = '.';
					}
				}
			}
		}
	}
	return bestMove;
}

// ===== UI =====
T4x4_UI::T4x4_UI() :UI<char>("Welcome to 4x4 Moving Tic-Tac-Toe!", 2) {}

Player<char>* T4x4_UI::create_player(string& name, char symbol, PlayerType type) {
	return new Player<char>(name, symbol, type);
}

void T4x4_UI::generate_computer_move(Player<char>* player, int& fr, int& fc, int& tr, int& tc) {
	T4x4_Board* board = dynamic_cast<T4x4_Board*>(player->get_board_ptr());
	if (!board) return;
	T4x4_Move mv = board->get_best_move(player->get_symbol(), player->get_symbol() == 'X' ? 'O' : 'X');
	fr = mv.fromRow; fc = mv.fromCol; tr = mv.toRow; tc = mv.toCol;
	if (fr == -1) { fr = 0; fc = 0; tr = 0; tc = 1; } 
}

Move<char>* T4x4_UI::get_move(Player<char>* player) {
	int fr, fc, tr, tc;
	char sym = player->get_symbol();
	if (player->is_human()) {
		cout << player->get_name() << " (" << player->get_symbol() << ") enter move:\n";
		cout << "From (row col): "; cin >> fr >> fc;
		cout << "To   (row col): "; cin >> tr >> tc;
	}
	else {
		generate_computer_move(player, fr, fc, tr, tc);
		cout << player->get_name() << " (Computer) moves from (" << fr << "," << fc << ") to (" << tr << "," << tc << ")\n";
	}
	return new T4x4_MoveObj(fr, fc, tr, tc, sym);
}
