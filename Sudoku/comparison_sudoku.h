#pragma once
#include "sudoku.h"

class ComparisonSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	friend std::istream& operator>>(std::istream& in, ComparisonSudoku& sudoku);

private:
	std::vector<std::pair<Position, Position>> Comparisons;
};
