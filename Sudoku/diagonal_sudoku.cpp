#include "stdafx.h"

#include "diagonal_sudoku.h"

#include "solver.h"

std::string_view DiagonalSudoku::GetName() const
{
	return "对角线数独"sv;
}

void DiagonalSudoku::InitializeSolver(Solver& solver)
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		const size_t j{i};
		ConstraintType diagonalConstraint{
			[](const NumType num, const Sudoku& sudoku)
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
			}
		};
		solver.AddConstraint(i, j, std::move(diagonalConstraint));
	}
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		const size_t j{COL_SIZE - i - 1};
		ConstraintType diagonalConstraint{
			[](const NumType num, const Sudoku& sudoku)
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
			}
		};
		solver.AddConstraint(i, j, std::move(diagonalConstraint));
	}
}
