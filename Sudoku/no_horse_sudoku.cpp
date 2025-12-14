#include "stdafx.h"

#include "no_horse_sudoku.h"

constexpr Sudoku::Position DIRECTIONS[8]{
	{1, 2},
	{1, -2},
	{2, 1},
	{2, -1},
	{-1, 2},
	{-1, -2},
	{-2, 1},
	{-2, -1}
};

std::string_view NoHorseSudoku::GetName() const
{
	return "无马数独"sv;
}

void NoHorseSudoku::InitializeExtraConstraints()
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			Constraint horseConstraint{
				[i, j, this](const NumType num, const BoardType& board)
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
						result &= num != board[K(newI, newJ)];
					}
					return result;
				}
			};
			(*ExtraConstraints)[K(i, j)].emplace_back(std::move(horseConstraint));
		}
	}
}
