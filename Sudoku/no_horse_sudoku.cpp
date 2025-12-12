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

void NoHorseSudoku::InitializeExtraConstraints()
{
    Super::InitializeExtraConstraints();

    for (size_t i = 0; i < ROW_SIZE; i++)
    {
        for (size_t j = 0; j < COL_SIZE; j++)
        {
            Constraint HorseConstraint = [i, j, this](int Num)
            {
                bool result = true;
                for (const Position &Direction : Directions)
                {
                    size_t newI = i + Direction.first;
                    size_t newJ = j + Direction.second;
                    if (newI < 0 || newI >= ROW_SIZE || newJ < 0 || newJ >= COL_SIZE)
                    {
                        continue;
                    }
                    result &= Num != Board[K(newI, newJ)];
                }
                return result;
            };
            ExtraConstraints[K(i, j)].emplace_back(std::move(HorseConstraint));
        }
    }
}
