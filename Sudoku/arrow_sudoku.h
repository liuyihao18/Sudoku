#pragma once
#include "sudoku.h"

class ArrowSudoku final : public Sudoku
{
	using Super = Sudoku;

	using Arrow = std::vector<Position>;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeConstraints() override;

	void Read(std::istream& in) override;

private:
	std::vector<Arrow> Arrows;
};
