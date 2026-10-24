#include "stdafx.h"

#include "diagonal_sudoku.h"

#include "solver.h"

namespace
{
	constexpr auto NAME{"对角线数独"sv};
}

std::string_view DiagonalSudoku::GetName() const
{
	return NAME;
}

void DiagonalSudoku::InitializeConstraints()
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		const size_t j{i};
		AddConstraint(i, j, [](const Sudoku& sudoku, const NumType num)
		{
			for (size_t ii{}; ii < ROW_SIZE; ii++)
			{
				if (const size_t jj{ii};
					num == sudoku(ii, jj))
				{
					return false;
				}
			}
			return true;
		});
	}
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		const size_t j{COL_SIZE - i - 1};
		AddConstraint(i, j, [](const Sudoku& sudoku, const NumType num)
		{
			for (size_t ii{}; ii < ROW_SIZE; ii++)
			{
				if (const size_t jj{COL_SIZE - ii - 1};
					num == sudoku(ii, jj))
				{
					return false;
				}
			}
			return true;
		});
	}
}
