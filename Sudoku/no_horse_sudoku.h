#pragma once
#include "sudoku.h"

class NoHorseSudoku final : public Sudoku
{
    using Super = Sudoku;

protected:
    virtual void InitializeExtraConstraints() override;
};
