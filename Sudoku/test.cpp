#include "stdafx.h"

#include "common.h"
#include "sudoku.h"

void TestStandardSudokus()
{
    std::string DirName = "standard/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestKillerSudokus()
{
    std::string DirName = "killer/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestThermometerSudokus()
{
    std::string DirName = "thermometer/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestOddEvenSudokus()
{
    std::string DirName = "odd_even/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestContinuousSudokus()
{
    std::string DirName = "continuous/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestNoHorseSudokus()
{
    std::string DirName = "no_horse/"s;
    for (size_t i = 1; i <= 5; i++)
    {
        std::string Filename = "sudoku"s + std::to_string(i) + ".txt"s;
        std::filesystem::path Path(DirName + Filename);
        SolveSudoku(Path);
    }
}

void TestAll()
{
    TestStandardSudokus();
    TestKillerSudokus();
    TestThermometerSudokus();
    TestOddEvenSudokus();
    TestContinuousSudokus();
    TestNoHorseSudokus();
}
