#include "sudoku.h"
#include <algorithm>

#define CHECK_ONCE

static_assert(ROW_SIZE == NUM_SIZE);
static_assert(COL_SIZE == NUM_SIZE);
static_assert(ROW_SIZE == SQUARE_SIZE * SQUARE_ROW_SIZE);
static_assert(COL_SIZE == SQUARE_SIZE * SQUARE_COL_SIZE);
static_assert(SQUARE_SIZE * SQUARE_SIZE == NUM_SIZE);

Sudoku::Sudoku() : Board(ROW_SIZE * COL_SIZE, 0),
				   RowConstraints(ROW_SIZE, 0), ColConstraints(COL_SIZE, 0),
				   SquareConstraints(SQUARE_SIZE * SQUARE_SIZE, 0)
{
}

bool Sudoku::Solve()
{
	InitializeConstraints();
	bool answer = true;
	if (!DFS(GetSpaces()))
	{
		answer = false;
		std::cerr << "*** 数独无解 ***" << std::endl;
	}
	return answer;
}

void Sudoku::InitializeConstraints()
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			int Num = Board[K(i, j)];
			RowConstraints[i] |= 1 << Num;
			ColConstraints[j] |= 1 << Num;
			SquareConstraints[SquareK(i, j)] |= 1 << Num;
		}
	}
}

bool Sudoku::CheckOnce(const std::vector<Position> &spaces)
{
	while (true)
	{
		bool Update = false;
		for (auto &&[i, j] : spaces)
		{
			if (Board[K(i, j)])
			{
				continue;
			}
			int TargetNum = 0;
			int Count = GetCandidateCount(i, j, TargetNum);
			if (Count == 1)
			{
				Board[K(i, j)] = TargetNum;
				AddConstraints(i, j, Board[K(i, j)]);
				Update = true;
			}
			else if (Count == 0)
			{
				return false;
			}
		}
		if (!Update)
		{
			break;
		}
	}
	return true;
}

bool Sudoku::DFS(const std::vector<Position> &spaces, size_t pos)
{
	if (pos == spaces.size())
	{
		return true;
	}
	auto &&[i, j] = spaces[pos];
#ifdef CHECK_ONCE
	if (!CheckOnce(spaces))
	{
		return false;
	}
	if (Board[K(i, j)])
	{
		return DFS(spaces, pos + 1);
	}
#endif
	for (int Num = 1; Num <= NUM_SIZE; Num++)
	{
		if (!SatisfyConstraints(i, j, Num))
		{
			continue;
		}
		Board[K(i, j)] = Num;
		AddConstraints(i, j, Board[K(i, j)]);
		if (DFS(spaces, pos + 1))
		{
			return true;
		}
#ifdef CHECK_ONCE
		size_t n = spaces.size();
		for (size_t p = pos; p < n; p++)
		{
			auto &&[i, j] = spaces[p];
			if (Board[K(i, j)])
			{
				RemoveConstraints(i, j, Board[K(i, j)]);
				Board[K(i, j)] = 0;
			}
		}
#else
		RemoveConstraints(i, j, Board[K(i, j)]);
		Board[K(i, j)] = 0;
#endif
	}
	return false;
}

bool Sudoku::SatisfyConstraints(size_t i, size_t j, int num)
{
	return !CheckRowConstraints(i, j, num) && !CheckColConstraints(i, j, num) && !CheckSquareConstraints(i, j, num);
}

void Sudoku::AddConstraints(size_t i, size_t j, int num)
{
	AddRowConstraints(i, j, num);
	AddColConstraints(i, j, num);
	AddSquareConstraints(i, j, num);
}

void Sudoku::RemoveConstraints(size_t i, size_t j, int num)
{
	RemoveRowConstraints(i, j, num);
	RemoveColConstraints(i, j, num);
	RemoveSquareConstraints(i, j, num);
}

int Sudoku::GetCandidateCount(size_t i, size_t j, int &TargetNum)
{
	int Count = 0;
	for (int Num = 1; Num <= NUM_SIZE; Num++)
	{
		if (SatisfyConstraints(i, j, Num))
		{
			Count++;
			TargetNum = Num;
		}
	}
	return Count;
}

std::vector<Sudoku::Position> Sudoku::GetSpaces()
{
	std::vector<Position> Spaces;
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			if (!Board[K(i, j)])
			{
				Spaces.emplace_back(i, j);
			}
		}
	}
#ifdef CHECK_ONCE
	std::sort(Spaces.begin(), Spaces.end(), [this](Position p1, Position p2) {
		int DevNull = 0;
		return GetCandidateCount(p1.first, p1.second, DevNull) < GetCandidateCount(p2.first, p2.second, DevNull);
		});
#endif
	return Spaces;
}

bool Sudoku::CheckRowConstraints(size_t i, size_t j, int num)
{
	return RowConstraints[i] & (1 << num);
}

bool Sudoku::CheckColConstraints(size_t i, size_t j, int num)
{
	return ColConstraints[j] & (1 << num);
}

bool Sudoku::CheckSquareConstraints(size_t i, size_t j, int num)
{
	return SquareConstraints[SquareK(i, j)] & (1 << num);
}

void Sudoku::AddRowConstraints(size_t i, size_t j, int num)
{
	RowConstraints[i] |= (1 << num);
}

void Sudoku::AddColConstraints(size_t i, size_t j, int num)
{
	ColConstraints[j] |= (1 << num);
}

void Sudoku::AddSquareConstraints(size_t i, size_t j, int num)
{
	SquareConstraints[SquareK(i, j)] |= (1 << num);
}

void Sudoku::RemoveRowConstraints(size_t i, size_t j, int num)
{
	RowConstraints[i] &= ~(1 << num);
}

void Sudoku::RemoveColConstraints(size_t i, size_t j, int num)
{
	ColConstraints[j] &= ~(1 << num);
}

void Sudoku::RemoveSquareConstraints(size_t i, size_t j, int num)
{
	SquareConstraints[SquareK(i, j)] &= ~(1 << num);
}

int &Sudoku::operator()(size_t i, size_t j)
{
	return Board[K(i, j)];
}

const int &Sudoku::operator()(size_t i, size_t j) const
{
	return Board[K(i, j)];
}

int &Sudoku::operator[](size_t k)
{
	return Board[k];
}

const int &Sudoku::operator[](size_t k) const
{
	return Board[k];
}

std::istream &operator>>(std::istream &in, Sudoku &sudoku)
{
	for (size_t k = 0; k < ROW_SIZE * COL_SIZE; k++)
	{
		in >> sudoku[k];
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
