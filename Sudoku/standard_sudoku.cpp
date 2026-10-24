#include "stdafx.h"

#include "standard_sudoku.h"

namespace
{
	constexpr auto NAME{"标准数独"sv};
}

std::string_view StandardSudoku::GetName() const
{
	return NAME;
}
