#pragma once
#include "sudoku.h"

class OddEvenSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	void Read(std::istream& in) override;

private:
	std::vector<Position> Odd;
	std::vector<Position> Even;
};
