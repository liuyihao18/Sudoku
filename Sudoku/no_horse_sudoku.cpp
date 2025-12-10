#include "no_horse_sudoku.h"

bool NoHorseSudoku::SatisfyConstraints(size_t i, size_t j, int num)
{
	return Super::SatisfyConstraints(i, j, num);
}

void NoHorseSudoku::AddConstraints(size_t i, size_t j, int num)
{
	Super::AddColConstraints(i, j, num);
}

void NoHorseSudoku::RemoveConstraints(size_t i, size_t j, int num)
{
	Super::RemoveConstraints(i, j, num);
}
