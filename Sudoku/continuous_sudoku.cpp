#include "stdafx.h"

#include "continuous_sudoku.h"

std::string_view ContinuousSudoku::GetName() const
{
	using std::string_view_literals::operator""sv;
	return "连续数独"sv;
}
