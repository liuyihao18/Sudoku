#pragma once
#include "sudoku.h"

class NoHorseSudoku final : public Sudoku
{
    using Super = Sudoku;

public:
    virtual std::string_view GetName() const override;

protected:
    virtual void InitializeExtraConstraints() override;
};
