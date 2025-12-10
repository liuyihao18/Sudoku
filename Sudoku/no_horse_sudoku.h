#pragma once
#include "sudoku.h"

class NoHorseSudoku : public Sudoku
{
	using Super = Sudoku;

protected:
	virtual bool SatisfyConstraints(size_t i, size_t j, int num) override;
	virtual void AddConstraints(size_t i, size_t j, int num) override;
	virtual void RemoveConstraints(size_t i, size_t j, int num) override;
};
