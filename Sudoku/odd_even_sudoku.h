#pragma once
#include "sudoku.h"

class OddEvenSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	friend std::istream& operator>>(std::istream& in, OddEvenSudoku& sudoku);

private:
	std::vector<Position> Odd;
	std::vector<Position> Even;
};
