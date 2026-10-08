#include "stdafx.h"

#include "solver.h"
#include "sudoku.h"
#include "thread_pool.h"

Solver::Solver()
{

}

bool Solver::Solve(Sudoku &sudoku)
{
    if (!CheckOnce(FindSpaces(sudoku), sudoku))
    {
        return false;
    }
    auto spaces = FindSpaces(sudoku);
    if (spaces.empty())
    {
        return true;
    }
    auto &&[i, j] = spaces[0];
    auto copySpaces = std::make_shared<const std::vector<Position>>(std::move(spaces));
    std::vector<std::pair<std::future<bool>, std::shared_ptr<Sudoku>>> results;
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (!SatisfyConstraints(i, j, num, sudoku))
        {
            continue;
        }
        auto copySudoku = std::make_shared<Sudoku>(sudoku);
        copySudoku->AddNum(i, j, num);
        results.emplace_back(
            ThreadPool::GetInstance().AddTask(
                [copySpaces, copySudoku, this]
                {
                    return Dfs(*copySpaces, 1, *copySudoku);
                }),
            copySudoku);
    }
    while (std::ranges::any_of(results,
                               [](const std::pair<std::future<bool>, std::shared_ptr<Sudoku>> &result)
                               {
                                   return result.first.valid();
                               }))
    {
        for (auto &&[result, copySudoku] : results)
        {
            if (!result.valid())
            {
                continue;
            }
            if (const std::future_status status = result.wait_for(std::chrono::milliseconds(1));
                status == std::future_status::ready && result.get())
            {
                sudoku = std::move(*copySudoku);
                return true;
            }
        }
    }
    return false;
}

bool Solver::SatisfyConstraints(const size_t i, const size_t j, NumType num, const Sudoku &sudoku) const
{
    return !sudoku.HasConflict(i, j, num) &&
           std::ranges::all_of(ExtraConstraints[K(i, j)],
                               [num, &sudoku](const ConstraintType &extraConstraint)
                               {
                                   return extraConstraint(num, sudoku);
                               });
}

size_t Solver::CalculateCandidateCount(const size_t i, const size_t j, NumType &targetNum, const Sudoku &sudoku) const
{
    size_t count{};
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (SatisfyConstraints(i, j, num, sudoku))
        {
            count++;
            targetNum = num;
        }
    }
    return count;
}

std::vector<Position> Solver::FindSpaces(const Sudoku &sudoku) const
{
    std::vector<Position> spaces;
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            if (!sudoku(i, j))
            {
                spaces.emplace_back(i, j);
            }
        }
    }
    return spaces;
}

void Solver::RestoreSpaces(const std::vector<Position> &spaces, size_t pos, Sudoku &sudoku) const
{
    for (const size_t n{spaces.size()}; pos < n; pos++)
    {
        if (auto &&[i, j]{spaces[pos]};
            sudoku(i, j))
        {
            sudoku.RemoveNum(i, j);
        }
    }
}

bool Solver::CheckOnce(const std::vector<Position> &spaces, Sudoku &sudoku) const
{
    bool checkOver{};
    while (!checkOver)
    {
        checkOver = true;
        for (auto &&[i, j] : spaces)
        {
            if (sudoku(i, j))
            {
                continue;
            }
            NumType targetNum{};
            if (const size_t count{CalculateCandidateCount(i, j, targetNum, sudoku)};
                count == 1)
            {
                sudoku.AddNum(i, j, targetNum);
                checkOver = false;
            }
            else if (count == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Solver::Dfs(const std::vector<Position> &spaces, const size_t pos, Sudoku &sudoku) const
{
    if (pos == spaces.size())
    {
        return true;
    }
    auto &&[i, j]{spaces[pos]};
    if (sudoku(i, j))
    {
        return Dfs(spaces, pos + 1, sudoku);
    }
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (!SatisfyConstraints(i, j, num, sudoku))
        {
            continue;
        }
        sudoku.AddNum(i, j, num);
        if (CheckOnce(spaces, sudoku))
        {
            if (Dfs(spaces, pos + 1, sudoku))
            {
                return true;
            }
        }
        RestoreSpaces(spaces, pos, sudoku);
    }
    return false;
}
