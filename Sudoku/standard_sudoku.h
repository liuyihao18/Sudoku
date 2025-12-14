#pragma once
#include "sudoku.h"

class StandardSudoku final : public Sudoku
{
	using Super = Sudoku;

public:
	std::string_view GetName() const override;
};
