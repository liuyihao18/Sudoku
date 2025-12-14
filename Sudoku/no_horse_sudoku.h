#pragma once
#include "sudoku.h"

class NoHorseSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	std::string_view GetName() const override;

protected:
	void InitializeExtraConstraints() override;
};
