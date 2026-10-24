#include "stdafx.h"

#include "solver.h"
#include "sudoku.h"
#include "thread_pool.h"

namespace
{
	constexpr bool ENABLE_MAX_START_NUM = false;
	constexpr bool ENABLE_RANDOM_SHUFFLE_NUMBERS = false;
}

bool Solver::Solve(Sudoku& sudoku)
{
	if (!CheckOnce(sudoku, sudoku.GetSpaces(), 0))
	{
		return false;
	}

	auto spaces = sudoku.GetSpaces();
	if (spaces.empty())
	{
		return true;
	}

	Position p;
	if constexpr (ENABLE_MAX_START_NUM)
	{
		std::vector<size_t> indexes(spaces.size());
		std::ranges::iota(indexes, 0);

		std::vector<size_t> candidateCounts(spaces.size());
		std::ranges::transform(
			spaces, std::begin(candidateCounts),
			[&sudoku](const Position& p)
			{
				auto [i, j]{p};
				NumType null;
				return CalculateCandidateCount(sudoku, i, j, null);
			});

		std::ranges::sort(indexes, [&candidateCounts](const size_t i1, const size_t i2)
		{
			return candidateCounts[i1] > candidateCounts[i2];
		});

		p = spaces[indexes[0]];
	}
	else
	{
		p = spaces[0];
	}

	auto [i, j]{p};
	auto solved = std::make_shared<std::atomic<bool>>(false);

	std::vector<std::pair<std::future<bool>, std::shared_ptr<Sudoku>>> results;
	for (NumType num{1}; num <= NUM_SIZE; num++)
	{
		if (!sudoku.SatisfyConstraints(i, j, num))
		{
			continue;
		}

		auto copySudoku = std::make_shared<Sudoku>(sudoku);
		copySudoku->AddNum(i, j, num);
		results.emplace_back(
			ThreadPool::GetInstance().AddTask(
				[copySudoku, solved]
				{
					return Dfs(*copySudoku, copySudoku->GetSpaces(), 0, solved);
				}),
			copySudoku);
	}

	while (std::ranges::any_of(results,
	                           [](const std::pair<std::future<bool>, std::shared_ptr<Sudoku>>& result)
	                           {
		                           return result.first.valid();
	                           }))
	{
		for (auto&& [result, copySudoku] : results)
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

size_t Solver::CalculateCandidateCount(const Sudoku& sudoku, const size_t i, const size_t j, NumType& targetNum)
{
	size_t count{};
	for (NumType num{1}; num <= NUM_SIZE; num++)
	{
		if (sudoku.SatisfyConstraints(i, j, num))
		{
			count++;
			targetNum = num;
		}
	}
	return count;
}

bool Solver::CheckOnce(Sudoku& sudoku, const std::vector<Position>& spaces, size_t pos)
{
	bool checkOver{};
	while (!checkOver)
	{
		checkOver = true;
		for (const size_t n{spaces.size()}; pos < n; pos++)
		{
			auto [i, j]{spaces[pos]};
			if (sudoku(i, j))
			{
				continue;
			}
			NumType targetNum{};
			if (const size_t count{CalculateCandidateCount(sudoku, i, j, targetNum)};
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

void Solver::RestoreSpaces(Sudoku& sudoku, const std::vector<Position>& spaces, size_t pos)
{
	for (const size_t n{spaces.size()}; pos < n; pos++)
	{
		if (auto&& [i, j]{spaces[pos]};
			sudoku(i, j))
		{
			sudoku.RemoveNum(i, j);
		}
	}
}

bool Solver::Dfs(Sudoku& sudoku, const std::vector<Position>& spaces, const size_t pos,
                 const std::shared_ptr<std::atomic<bool>>& solved)
{
	if (*solved)
	{
		return false;
	}
	if (pos == spaces.size())
	{
		*solved = true;
		return true;
	}

	auto&& [i, j]{spaces[pos]};
	if (sudoku(i, j))
	{
		return Dfs(sudoku, spaces, pos + 1, solved);
	}

	std::array<NumType, 9> numbers;
	std::ranges::iota(numbers, 1);

	if constexpr (ENABLE_RANDOM_SHUFFLE_NUMBERS)
	{
		std::random_device rd{};
		std::mt19937 g(rd());
		std::ranges::shuffle(numbers, g);
	}

	for (const NumType num : numbers)
	{
		if (!sudoku.SatisfyConstraints(i, j, num))
		{
			continue;
		}
		sudoku.AddNum(i, j, num);
		if (CheckOnce(sudoku, spaces, pos + 1) && Dfs(sudoku, spaces, pos + 1, solved))
		{
			return true;
		}
		RestoreSpaces(sudoku, spaces, pos);
	}

	return false;
}
