#pragma once
#include "sudoku.h"

class ComparisonSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	void Read(std::istream& in) override;

private:
	std::vector<std::pair<Position, Position>> Comparisons;
};
