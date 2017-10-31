#ifndef SUDOKU_H
#define SUDOKU_H
#include <array>
#include <iosfwd>

class Sudoku
{
public:
	Sudoku() {
		for (auto &row : grid) {
			row.fill(AVAILABLE);
		}
	}

	bool solve() {
		int row, col;
		if (!next_available(row, col)) {
			return true; // solved
		}
		for (int value = 1; value <= GRID_SIZE; ++value) {
			if (check_move(row, col, value)) {
				grid[row][col] = value;
				if (solve()) {
					return true; // unwind
				}
				grid[row][col] = AVAILABLE;
			}
		}
		return false; // backtrack
	}

	friend std::istream& operator>>(std::istream &is, Sudoku &game) {
		for (auto &row : game.grid) {
			for (auto &col : row) {
				if (!(is >> col)) {
					std::cerr << "stream input failed\n";
					return is;
				}
			}
		}
		return is;
	}

	friend std::ostream& operator<<(std::ostream &os, const Sudoku &game) {
		for (auto &row : game.grid) {
			for (auto col : row) {
				os << col << ' ';
			}
			os << '\n';
		}
		return os;
	}

private:
	enum { AVAILABLE = 0, BOX_SIZE = 3, GRID_SIZE = 9 };

	bool next_available(int &row, int &col) const {
		for (row = 0; row < GRID_SIZE; ++row) {
			for (col = 0; col < GRID_SIZE; ++col) {
				if (grid[row][col] == AVAILABLE) return true;
			}
		}
		return false;
	}

	bool check_move(int row, int col, int value) const {
		return check_line(row, col, value) && check_box(row, col, value);
	}

	bool check_line(int row, int col, int value) const {
		for (int i = 0; i < GRID_SIZE; ++i) {
			if (grid[row][i] == value) { // horizontal
				return false;
			}
			if (grid[i][col] == value) { // vertical
				return false;
			}
		}
		return true;
	}

	bool check_box(int row, int col, int value) const {
		row -= row % BOX_SIZE; // [0, 3, 6, ...]
		col -= col % BOX_SIZE; // [0, 3, 6, ...]
		for (int r = 0; r < BOX_SIZE; ++r) {
			for (int c = 0; c < BOX_SIZE; ++c) {
				if (grid[row + r][col + c] == value) {
					return false;
				}
			}
		}
		return true;
	}

	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;
};
#endif // SUDOKU_H
