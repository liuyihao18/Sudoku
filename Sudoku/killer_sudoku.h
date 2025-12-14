#pragma once
#include "sudoku.h"

struct Killer
{
	NumType Sum{};
	std::vector<Position> Positions;
};

class KillerSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeSolver(Solver& solver) override;

	friend std::istream& operator>>(std::istream& in, KillerSudoku& sudoku);

private:
	std::vector<Killer> Killers;
};
