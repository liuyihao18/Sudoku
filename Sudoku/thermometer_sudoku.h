#pragma once
#include "sudoku.h"

class ThermometerSudoku : public Sudoku
{
	friend std::istream& operator>>(std::istream& in, ThermometerSudoku& sudoku);
};
