#pragma once
#include "sudoku.h"

class KillerSudoku final : public Sudoku
{
	using Super = Sudoku;

	struct Killer
	{
		NumType Sum{};
		std::vector<Position> Positions;
	};

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeConstraints() override;

	void Read(std::istream& in) override;

private:
	std::vector<Killer> Killers;
};
