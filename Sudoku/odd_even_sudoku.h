#pragma once
#include "sudoku.h"

class OddEvenSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	std::string_view GetName() const override;

protected:
	std::vector<Position> Odd;
	std::vector<Position> Even;
	void InitializeExtraConstraints() override;

public:
	friend std::istream& operator>>(std::istream& in, OddEvenSudoku& sudoku);
};
