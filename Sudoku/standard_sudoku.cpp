#include "stdafx.h"

#include "standard_sudoku.h"

std::string_view StandardSudoku::GetName() const
{
    using std::string_view_literals::operator""sv;
    return "标准数独"sv;
}
