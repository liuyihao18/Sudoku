#pragma once
#include "sudoku.h"

class ThermometerSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	friend std::istream& operator>>(std::istream& in, ThermometerSudoku& sudoku);

private:
	std::vector<std::vector<Position>> Thermometers;
};
