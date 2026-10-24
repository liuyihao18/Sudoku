#pragma once
#include "sudoku.h"

class WindowSudoku final : public Sudoku
{
	using Super = Sudoku;

	using Window = std::pair<Position, Position>;

public:
	std::string_view GetName() const override;
	void InitializeConstraints() override;

	void Read(std::istream &in) override;

private:
	std::vector<Window> Windows;
};
