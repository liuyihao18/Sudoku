#include "sudoku.h"
#include <algorithm>

static_assert(ROW_SIZE == NUM_SIZE);
static_assert(COL_SIZE == NUM_SIZE);
static_assert(ROW_SIZE == INDEX_SIZE * ROW_INDEX);
static_assert(COL_SIZE == INDEX_SIZE * COL_INDEX);
static_assert(INDEX_SIZE * INDEX_SIZE == NUM_SIZE);

Sudoku::Sudoku() : _board(ROW_SIZE * COL_SIZE, -1), _candidates(ROW_SIZE * COL_SIZE)
{
}

int &Sudoku::operator()(size_t i, size_t j)
{
	return _board[k(i, j)];
}

const int &Sudoku::operator()(size_t i, size_t j) const
{
	return _board[k(i, j)];
}

bool Sudoku::solve()
{
	_initializeCandidates();
	std::vector<int> copyBoard = _board;
	std::vector<int> copyCandidates = _candidates;
	bool result = _solve(copyBoard, copyCandidates);
	_board = std::move(copyBoard);
	_candidates = std::move(copyCandidates);
	return result;
}

bool Sudoku::_solve(std::vector<int> &board, std::vector<int> &candidates)
{
	// 先把确定的填上
	while (true)
	{
		bool canUpdate = false;
		for (size_t i = 0; i < ROW_SIZE; i++)
		{
			for (size_t j = 0; j < COL_SIZE; j++)
			{
				if (board[k(i, j)] != 0)
				{
					continue;
				}
				if (_isOneCandidate(candidates[k(i, j)]))
				{
					board[k(i, j)] = _getOneCandidate(candidates[k(i, j)]);
					_updateCandidates(i, j, board[k(i, j)], board, candidates);
					canUpdate = true;
					break;
				}
				else if (candidates[k(i, j)] == 0)
				{
					return false;
				}
			}
			if (canUpdate)
			{
				break;
			}
		}
		if (!canUpdate)
		{
			break;
		}
	}
	// 然后再试错
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			if (board[k(i, j)] != 0)
			{
				continue;
			}
			for (int candidate : _getAllCandidates(candidates[k(i, j)]))
			{
				std::vector<int> tempBoard = board;
				std::vector<int> tempCandidates = candidates;
				tempBoard[k(i, j)] = candidate;
				_updateCandidates(i, j, tempBoard[k(i, j)], tempBoard, tempCandidates);
				if (_solve(tempBoard, tempCandidates))
				{
					board = std::move(tempBoard);
					candidates = std::move(tempCandidates);
					return true;
				}
			}
			return false;
		}
	}
	if (std::any_of(board.begin(), board.end(), [](int num)
					{ return num == 0; }))
	{
		std::cerr << "*** 数独无解 ***" << std::endl;
	}
	return true;
}

void Sudoku::_initializeCandidates()
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			if (_board[k(i, j)] == 0)
			{
				_candidates[k(i, j)] = 0x3fe;
			}
		}
	}
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			if (_board[k(i, j)] != 0)
			{
				_updateCandidates(i, j, _board[k(i, j)], _board, _candidates);
			}
		}
	}
}

bool Sudoku::_isOneCandidate(int candidates)
{
	return candidates == (1 << 1) || candidates == (1 << 2) || candidates == (1 << 3) || candidates == (1 << 4) || candidates == (1 << 5) || candidates == (1 << 6) || candidates == (1 << 7) || candidates == (1 << 8) || candidates == (1 << 9);
}

int Sudoku::_getOneCandidate(int candidates)
{
	for (int i = 1; i <= NUM_SIZE; i++)
	{
		if (candidates & (1 << i))
		{
			return i;
		}
	}
	return -1;
}

std::vector<int> Sudoku::_getAllCandidates(int candidates)
{
	std::vector<int> all_candidates;
	for (int i = 1; i <= NUM_SIZE; i++)
	{
		if (candidates & (1 << i))
		{
			all_candidates.push_back(i);
		}
	}
	return all_candidates;
}

void Sudoku::_updateCandidates(size_t i, size_t j, int num, const std::vector<int> &board, std::vector<int> &candidates)
{
	// i 行
	for (size_t _j = 0; _j < COL_SIZE; _j++)
	{
		if (board[k(i, _j)] == 0)
		{
			candidates[k(i, _j)] &= ~(1 << num);
		}
	}
	// j 列
	for (size_t _i = 0; _i < ROW_SIZE; _i++)
	{
		if (board[k(_i, j)] == 0)
		{
			candidates[k(_i, j)] &= ~(1 << num);
		}
	}
	// 宫
	size_t index_i = (i / INDEX_SIZE) * INDEX_SIZE;
	size_t index_j = (j / INDEX_SIZE) * INDEX_SIZE;
	for (size_t _i = 0; _i < INDEX_SIZE; _i++)
	{
		for (size_t _j = 0; _j < INDEX_SIZE; _j++)
		{
			if (board[k(index_i + _i, index_j + _j)] == 0)
			{
				candidates[k(index_i + _i, index_j + _j)] &= ~(1 << num);
			}
		}
	}
}

std::istream &operator>>(std::istream &in, Sudoku &sudoku)
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			in >> sudoku(i, j);
		}
	}
	return in;
}

std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku)
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			out << sudoku(i, j) << " ";
		}
		out << std::endl;
	}
	return out;
}
