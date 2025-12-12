#pragma once
#include "sudoku.h"

class ContinuousSudoku final : public Sudoku
{
    using Super = Sudoku;

public:
    virtual std::string_view GetName() const override;

protected:
    std::vector<std::vector<Position>> ContinuousPositions;
    virtual void InitializeExtraConstraints() override;

public:
    friend std::istream &operator>>(std::istream &in, ContinuousSudoku &sudoku);
};
