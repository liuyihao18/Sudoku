#pragma once
#include "sudoku.h"

class NoHorseSudoku : public Sudoku
{
    using Super = Sudoku;

protected:
    virtual void InitializeConstraints() override;
};
