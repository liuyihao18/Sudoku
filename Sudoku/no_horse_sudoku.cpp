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

bool NoHorseSudoku::SatisfyConstraints(size_t i, size_t j, int num)
{
    return Super::SatisfyConstraints(i, j, num) && !CheckHorseConstraints(i, j, num);
}

bool NoHorseSudoku::CheckHorseConstraints(size_t i, size_t j, int num)
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
        result |= num == Board[K(newI, newJ)];
    }
    return result;
}
