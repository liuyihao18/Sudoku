#pragma once
#include "sudoku.h"

class ThermometerSudoku final : public Sudoku
{
	using Super = Sudoku;

protected:
	virtual void InitializeExtraConstraints() override;

protected:
	std::vector<std::vector<Position>> Thermometers;

public:
	friend std::istream &operator>>(std::istream &in, ThermometerSudoku &sudoku);
};
