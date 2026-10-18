#include "stdafx.h"

#include "sudoku.h"

std::string_view Sudoku::GetName() const
{
	return "数独"sv;
}

void Sudoku::InitializeSolver(Solver& solver)
{
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
