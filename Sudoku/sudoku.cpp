#include "sudoku.h"
#include <algorithm>

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
	if (!CheckOnce(GetSpaces()) || !DFS(GetSpaces()))
	{
		std::cerr << "*** 数独无解 ***" << std::endl;
		return false;
	}
	return true;
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

bool Sudoku::CheckOnce(const std::vector<Position> &Spaces)
{
	bool Update = true;
	while (Update)
	{
		Update = false;
		for (auto &&[i, j] : Spaces)
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
	}
	return true;
}

bool Sudoku::DFS(const std::vector<Position> &Spaces, size_t pos)
{
	if (pos == Spaces.size())
	{
		return true;
	}
	auto &&[i, j] = Spaces[pos];
	if (Board[K(i, j)])
	{
		return DFS(Spaces, pos + 1);
	}
	for (int Num = 1; Num <= NUM_SIZE; Num++)
	{
		if (!SatisfyConstraints(i, j, Num))
		{
			continue;
		}
		Board[K(i, j)] = Num;
		AddConstraints(i, j, Board[K(i, j)]);
		if (CheckOnce(Spaces) && DFS(Spaces, pos + 1))
		{
			return true;
		}
		RestorSpaces(Spaces, pos);
	}
	return false;
}

bool Sudoku::SatisfyConstraints(size_t i, size_t j, int Num)
{
	return !CheckRowConstraints(i, j, Num) && !CheckColConstraints(i, j, Num) && !CheckSquareConstraints(i, j, Num);
}

void Sudoku::AddConstraints(size_t i, size_t j, int Num)
{
	AddRowConstraints(i, j, Num);
	AddColConstraints(i, j, Num);
	AddSquareConstraints(i, j, Num);
}

void Sudoku::RemoveConstraints(size_t i, size_t j, int Num)
{
	RemoveRowConstraints(i, j, Num);
	RemoveColConstraints(i, j, Num);
	RemoveSquareConstraints(i, j, Num);
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
	std::sort(Spaces.begin(), Spaces.end(), [this](Position p1, Position p2) {
		int DevNull = 0;
		return GetCandidateCount(p1.first, p1.second, DevNull) < GetCandidateCount(p2.first, p2.second, DevNull);
		});
	return Spaces;
}

void Sudoku::RestorSpaces(const std::vector<Position>& Spaces, size_t pos)
{
	size_t n = Spaces.size();
	for (; pos < n; pos++)
	{
		auto&& [i, j] = Spaces[pos];
		if (Board[K(i, j)])
		{
			RemoveConstraints(i, j, Board[K(i, j)]);
			Board[K(i, j)] = 0;
		}
	}
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
