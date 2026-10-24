#pragma once
#include "sudoku.h"

class ContinuousSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeConstraints() override;

	void Read(std::istream& in) override;

private:
	std::vector<std::pair<Position, Position>> ContinuousPositions;
};
