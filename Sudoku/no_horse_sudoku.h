#pragma once
#include "sudoku.h"

class NoHorseSudoku : public Sudoku
{
    using Super = Sudoku;

protected:
    virtual bool SatisfyConstraints(size_t i, size_t j, int Num) override;

private:
    bool CheckHorseConstraints(size_t i, size_t j, int Num);
};
