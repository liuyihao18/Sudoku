#pragma once
#include "sudoku.h"

struct Killer
{
    int Sum{};
    std::vector<Sudoku::Position> Positions;
};

class KillerSudoku final : public Sudoku
{
    using Super = Sudoku;

public:
    virtual std::string_view GetName() const override;

protected:
    std::vector<Killer> Killers;
    virtual void InitializeExtraConstraints() override;

public:
    friend std::istream &operator>>(std::istream &in, KillerSudoku &sudoku);
};