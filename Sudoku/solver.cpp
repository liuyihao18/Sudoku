#include "stdafx.h"

#include "solver.h"
#include "sudoku.h"
#include "thread_pool.h"

Solver::Solver()
{
    ExtraConstraints = std::make_shared<ConstraintsType>();
}

bool Solver::Solve(Sudoku &sudoku) const
{
    if (!CheckOnce(FindSpaces(sudoku, *ExtraConstraints), sudoku, *ExtraConstraints))
    {
        return false;
    }
    auto spaces = FindSpaces(sudoku, *ExtraConstraints);
    if (spaces.empty())
    {
        return true;
    }
    auto &&[i, j] = spaces[0];
    auto copySpaces = std::make_shared<const std::vector<Position>>(std::move(spaces));
    std::shared_ptr copyExtraConstraints = ExtraConstraints;
    std::vector<std::pair<std::future<bool>, std::shared_ptr<Sudoku>>> results;
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (!SatisfyConstraints(i, j, num, sudoku, *ExtraConstraints))
        {
            continue;
        }
        auto copySudoku = std::make_shared<Sudoku>(sudoku);
        copySudoku->AddNum(i, j, num);
        results.emplace_back(
            ThreadPool::GetInstance().AddTask(
                [copySpaces, copySudoku, copyExtraConstraints]
                {
                    return Dfs(*copySpaces, 1, *copySudoku, *copyExtraConstraints);
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

bool Solver::SatisfyConstraints(const size_t i, const size_t j, NumType num, const Sudoku &sudoku,
                                const ConstraintsType &extraConstraints)
{
    return !sudoku.HasNum(i, j, num) &&
           std::ranges::all_of(extraConstraints[K(i, j)],
                               [num, &sudoku](const Constraint &extraConstraint)
                               {
                                   return extraConstraint(num, sudoku);
                               });
}

size_t Solver::CalculateCandidateCount(const size_t i, const size_t j, NumType &targetNum, const Sudoku &sudoku,
                                       const ConstraintsType &extraConstraints)
{
    size_t count{};
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (SatisfyConstraints(i, j, num, sudoku, extraConstraints))
        {
            count++;
            targetNum = num;
        }
    }
    return count;
}

std::vector<Position> Solver::FindSpaces(const Sudoku &sudoku, const ConstraintsType &)
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

void Solver::RestoreSpaces(const std::vector<Position> &spaces, size_t pos, Sudoku &sudoku,
                           const ConstraintsType &)
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

bool Solver::CheckOnce(const std::vector<Position> &spaces, Sudoku &sudoku, const ConstraintsType &extraConstraints)
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
            if (const size_t count{CalculateCandidateCount(i, j, targetNum, sudoku, extraConstraints)};
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

bool Solver::Dfs(const std::vector<Position> &spaces, const size_t pos, Sudoku &sudoku,
                 const ConstraintsType &extraConstraints)
{
    if (pos == spaces.size())
    {
        return true;
    }
    auto &&[i, j]{spaces[pos]};
    if (sudoku(i, j))
    {
        return Dfs(spaces, pos + 1, sudoku, extraConstraints);
    }
    for (NumType num{1}; num <= NUM_SIZE; num++)
    {
        if (!SatisfyConstraints(i, j, num, sudoku, extraConstraints))
        {
            continue;
        }
        sudoku.AddNum(i, j, num);
        if (CheckOnce(spaces, sudoku, extraConstraints) && Dfs(spaces, pos + 1, sudoku, extraConstraints))
        {
            return true;
        }
        RestoreSpaces(spaces, pos, sudoku, extraConstraints);
    }
    return false;
}
