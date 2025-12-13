#include "stdafx.h"

#include "common.h"
#include "test.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        SolveSudoku(argv[1]);
    }
    else
    {
        TestAll();
    }
    return 0;
}
