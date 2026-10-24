#include "stdafx.h"

#include "sudoku.h"

std::string_view Sudoku::GetName() const
{
	return "数独"sv;
}

void Sudoku::InitializeConstraints()
{
}

std::vector<Position> Sudoku::GetSpaces() const
{
	std::vector<Position> spaces;
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			if (!Board[K(i, j)])
			{
				spaces.emplace_back(i, j);
			}
		}
	}
	return spaces;
}

bool Sudoku::SatisfyConstraints(const size_t i, const size_t j, NumType num) const
{
	return !HasConflict(i, j, num) &&
		std::ranges::all_of(ExtraConstraints[K(i, j)],
		                    [this, num](const ConstraintType& extraConstraint)
		                    {
			                    return extraConstraint(*this, num);
		                    });
}

void Sudoku::Read(std::istream& in)
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			NumType num{};
			in >> num;
			AddNum(i, j, num);
		}
	}
}

void Sudoku::Print(std::ostream& out) const
{
	std::ostringstream os;
	os << "-> "sv << GetName() << '\n';
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			os << Board[K(i, j)] << " "sv;
		}
		os << '\n';
	}
	out << os.str();
}

std::istream& operator>>(std::istream& in, Sudoku& sudoku)
{
	sudoku.Read(in);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
{
	sudoku.Print(out);
	return out;
}
