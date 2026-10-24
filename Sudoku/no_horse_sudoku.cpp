#include "stdafx.h"

#include "no_horse_sudoku.h"

#include "solver.h"

namespace
{
	constexpr auto NAME{"无马数独"sv};

	constexpr Position DIRECTIONS[8]{
		{1, 2},
		{1, -2},
		{2, 1},
		{2, -1},
		{-1, 2},
		{-1, -2},
		{-2, 1},
		{-2, -1}
	};
}

std::string_view NoHorseSudoku::GetName() const
{
	return NAME;
}

void NoHorseSudoku::InitializeConstraints()
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			AddConstraint(i, j, [i, j](const Sudoku& sudoku, const NumType num)
			{
				bool result{true};
				for (const auto& [deltaI, deltaJ] : DIRECTIONS)
				{
					const size_t newI{i + deltaI};
					const size_t newJ{j + deltaJ};
					if (newI < 0 || newI >= ROW_SIZE || newJ < 0 || newJ >= COL_SIZE)
					{
						continue;
					}
					result &= num != sudoku(newI, newJ);
				}
				return result;
			});
		}
	}
}
