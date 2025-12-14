#include "stdafx.h"

#include "sudoku.h"

std::string_view Sudoku::GetName() const
{
	return "数独"sv;
}

void Sudoku::InitializeSolver(Solver& solver)
{
}

std::istream& operator>>(std::istream& in, Sudoku& sudoku)
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			NumType num{};
			in >> num;
			sudoku.AddNum(i, j, num);
		}
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
{
	std::ostringstream os;
	os << "-> "sv << sudoku.GetName() << '\n';
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			os << sudoku(i, j) << " "sv;
		}
		os << '\n';
	}
	out << os.str();
	return out;
}
