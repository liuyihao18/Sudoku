#include "sudoku.h"
#include <algorithm>
#include <ranges>

static_assert(ROW_SIZE == NUM_SIZE);
static_assert(COL_SIZE == NUM_SIZE);
static_assert(ROW_SIZE == SQUARE_SIZE * SQUARE_ROW_SIZE);
static_assert(COL_SIZE == SQUARE_SIZE * SQUARE_COL_SIZE);
static_assert(SQUARE_SIZE * SQUARE_SIZE == NUM_SIZE);

Sudoku::Sudoku() : Board(ROW_SIZE * COL_SIZE),
				   RowNums(ROW_SIZE), ColNums(COL_SIZE),
				   SquareNums(SQUARE_SIZE * SQUARE_SIZE),
				   ExtraConstraints(ROW_SIZE * COL_SIZE)
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
}

bool Sudoku::CheckOnce(const std::vector<Position> &Spaces)
{
	bool CanUpdate = true;
	while (CanUpdate)
	{
		CanUpdate = false;
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
				AddNum(i, j, TargetNum);
				CanUpdate = true;
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
		AddNum(i, j, Num);
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
	return !RowHasNum(i, j, Num) && !ColHasNum(i, j, Num) && !SquareHasNum(i, j, Num) &&
		   std::ranges::all_of(ExtraConstraints[K(i, j)],
							   [=](const Constraint &ExtraConstraint)
							   { return ExtraConstraint(Num); });
}

void Sudoku::AddNum(size_t i, size_t j, int Num)
{
	Board[K(i, j)] = Num;
	AddRowNum(i, j, Num);
	AddColNum(i, j, Num);
	AddSquareNum(i, j, Num);
}

void Sudoku::RemoveNum(size_t i, size_t j)
{
	int Num = Board[K(i, j)];
	RemoveRowNum(i, j, Num);
	RemoveColNum(i, j, Num);
	RemoveSquareNum(i, j, Num);
	Board[K(i, j)] = 0;
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
	std::ranges::sort(Spaces,
					  [this](Position p1, Position p2)
					  {
						  int DevNull = 0;
						  return GetCandidateCount(p1.first, p1.second, DevNull) < GetCandidateCount(p2.first, p2.second, DevNull);
					  });
	return Spaces;
}

void Sudoku::RestorSpaces(const std::vector<Position> &Spaces, size_t pos)
{
	size_t n = Spaces.size();
	for (; pos < n; pos++)
	{
		auto &&[i, j] = Spaces[pos];
		if (Board[K(i, j)])
		{
			RemoveNum(i, j);
		}
	}
}

std::istream &operator>>(std::istream &in, Sudoku &sudoku)
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			int Num = 0;
			in >> Num;
			sudoku.AddNum(i, j, Num);
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
