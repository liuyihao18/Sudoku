#pragma once
#include "sudoku.h"

struct Killer
{
	NumType Sum{};
	std::vector<Sudoku::Position> Positions;
};

class KillerSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	std::string_view GetName() const override;

protected:
	std::vector<Killer> Killers;
	void InitializeExtraConstraints() override;

public:
	friend std::istream& operator>>(std::istream& in, KillerSudoku& sudoku);
};
