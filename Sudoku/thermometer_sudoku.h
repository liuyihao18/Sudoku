#pragma once
#include "sudoku.h"

class ThermometerSudoku final : public Sudoku
{
    using Super = Sudoku;

public:
    virtual std::string_view GetName() const override;

protected:
    std::vector<std::vector<Position>> Thermometers;
    virtual void InitializeExtraConstraints() override;

public:
    friend std::istream &operator>>(std::istream &in, ThermometerSudoku &sudoku);
};
