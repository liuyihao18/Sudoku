#pragma once
#include "sudoku.h"

class ThermometerSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	std::string_view GetName() const override;

protected:
	std::vector<std::vector<Position>> Thermometers;
	void InitializeExtraConstraints() override;

public:
	friend std::istream& operator>>(std::istream& in, ThermometerSudoku& sudoku);
};
