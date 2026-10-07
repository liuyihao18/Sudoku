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
		size_t j{i};
		Constraint diagonalConstraint{
			[](const NumType num, const Sudoku& sudoku)
			{
				for (size_t i{}; i < ROW_SIZE; i++)
				{
					size_t j{i};
					if (num == sudoku(i, j))
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
		size_t j{COL_SIZE - i - 1};
		Constraint diagonalConstraint{
			[](const NumType num, const Sudoku& sudoku)
			{
				for (size_t i{}; i < ROW_SIZE; i++)
				{
					size_t j{COL_SIZE - i - 1};
					if (num == sudoku(i, j))
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
