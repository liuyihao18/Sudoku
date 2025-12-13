#include "stdafx.h"

#include "common.h"
#include "test.h"

int main()
{
    // TestAll();
    SolveSudoku(R"(standard/sudoku.txt)"s);
    return 0;
}
