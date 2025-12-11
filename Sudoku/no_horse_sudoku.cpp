#include "no_horse_sudoku.h"

constexpr const Sudoku::Position Directions[8] = {
    {1, 2},
    {1, -2},
    {2, 1},
    {2, -1},
    {-1, 2},
    {-1, -2},
    {-2, 1},
    {-2, -1}};

bool NoHorseSudoku::SatisfyConstraints(size_t i, size_t j, int Num)
{
    return Super::SatisfyConstraints(i, j, Num) && !CheckHorseConstraints(i, j, Num);
}

bool NoHorseSudoku::CheckHorseConstraints(size_t i, size_t j, int Num)
{
    bool result = false;
    for (const Position &Direction : Directions)
    {
        size_t newI = i + Direction.first;
        size_t newJ = j + Direction.second;
        if (newI < 0 || newI >= ROW_SIZE || newJ < 0 || newJ >= COL_SIZE)
        {
            continue;
        }
        result |= Num == Board[K(newI, newJ)];
    }
    return result;
}
