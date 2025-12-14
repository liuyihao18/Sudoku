#include "stdafx.h"

#include "sudoku.h"

#include "thread_pool.h"

Sudoku::Sudoku()
{
	ExtraConstraints = std::make_shared<ExtraConstraintsType>();
}

bool Sudoku::Solve()
{
	InitializeExtraConstraints();
	if (!CheckOnce(FindSpaces(BoardState, *ExtraConstraints), BoardState, *ExtraConstraints) || !ThreadDfs())
	{
		std::cerr << "*** 数独无解 ***\n"sv;
		return false;
	}
	return true;
}

std::string_view Sudoku::GetName() const
{
	return "数独"sv;
}

void Sudoku::InitializeExtraConstraints()
{
}

bool Sudoku::ThreadDfs()
{
	auto spaces = FindSpaces(BoardState, *ExtraConstraints);
	if (spaces.empty())
	{
		return true;
	}
	auto&& [i, j] = spaces[0];
	auto copySpaces = std::make_shared<const std::vector<Position>>(std::move(spaces));
	auto copyExtraConstraints = ExtraConstraints;
	std::vector<std::pair<std::future<bool>, std::shared_ptr<FBoardState>>> results;
	for (NumType num{1}; num <= NUM_SIZE; num++)
	{
		if (!SatisfyConstraints(i, j, num, BoardState, *ExtraConstraints))
		{
			continue;
		}
		auto copyState = std::make_shared<FBoardState>(BoardState);
		AddNum(i, j, num, *copyState);
		results.emplace_back(
			ThreadPool::GetInstance().AddTask(
				[copySpaces, copyState, copyExtraConstraints]
				{
					return Dfs(*copySpaces, 1, *copyState, *copyExtraConstraints);
				}),
			copyState);
	}
	while (std::ranges::any_of(results,
	                           [](const std::pair<std::future<bool>, std::shared_ptr<FBoardState>>& Result)
	                           {
		                           return Result.first.valid();
	                           }))
	{
		for (auto&& [result, boardState] : results)
		{
			if (!result.valid())
			{
				continue;
			}
			if (const std::future_status status = result.wait_for(std::chrono::milliseconds(1));
				status == std::future_status::ready && result.get())
			{
				BoardState = *boardState;
				return true;
			}
		}
	}
	return false;
}

bool Sudoku::SatisfyConstraints(const size_t i, const size_t j, NumType num, const FBoardState& boardState,
                                const ExtraConstraintsType& extraConstraints)
{
	return !HasNum(i, j, num, boardState) &&
		std::ranges::all_of(extraConstraints[K(i, j)],
		                    [num, &boardState](const Constraint& extraConstraint)
		                    {
			                    return extraConstraint(num, boardState.Board);
		                    });
}

size_t Sudoku::CalculateCandidateCount(const size_t i, const size_t j, NumType& targetNum,
                                       const FBoardState& boardState,
                                       const ExtraConstraintsType& extraConstraints)
{
	size_t count{};
	for (NumType num{1}; num <= NUM_SIZE; num++)
	{
		if (SatisfyConstraints(i, j, num, boardState, extraConstraints))
		{
			count++;
			targetNum = num;
		}
	}
	return count;
}

std::vector<Sudoku::Position> Sudoku::FindSpaces(const FBoardState& boardState,
                                                 const ExtraConstraintsType&)
{
	std::vector<Position> spaces;
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			if (!boardState.Board[K(i, j)])
			{
				spaces.emplace_back(i, j);
			}
		}
	}
	return spaces;
}

void Sudoku::RestoreSpaces(const std::vector<Position>& spaces, size_t pos, FBoardState& boardState,
                           const ExtraConstraintsType&)
{
	for (const size_t n{spaces.size()}; pos < n; pos++)
	{
		if (auto&& [i, j]{spaces[pos]};
			boardState.Board[K(i, j)])
		{
			RemoveNum(i, j, boardState);
		}
	}
}

bool Sudoku::CheckOnce(const std::vector<Position>& spaces, FBoardState& boardState,
                       const ExtraConstraintsType& extraConstraints)
{
	bool checkOver{};
	while (!checkOver)
	{
		checkOver = true;
		for (auto&& [i, j] : spaces)
		{
			if (boardState.Board[K(i, j)])
			{
				continue;
			}
			NumType targetNum{};
			if (const size_t count{CalculateCandidateCount(i, j, targetNum, boardState, extraConstraints)}; 
				count == 1)
			{
				AddNum(i, j, targetNum, boardState);
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

bool Sudoku::Dfs(const std::vector<Position>& spaces, const size_t pos, FBoardState& boardState,
                 const ExtraConstraintsType& extraConstraints)
{
	if (pos == spaces.size())
	{
		return true;
	}
	auto&& [i, j]{spaces[pos]};
	if (boardState.Board[K(i, j)])
	{
		return Dfs(spaces, pos + 1, boardState, extraConstraints);
	}
	for (NumType num{1}; num <= NUM_SIZE; num++)
	{
		if (!SatisfyConstraints(i, j, num, boardState, extraConstraints))
		{
			continue;
		}
		AddNum(i, j, num, boardState);
		if (CheckOnce(spaces, boardState, extraConstraints) && Dfs(spaces, pos + 1, boardState, extraConstraints))
		{
			return true;
		}
		RestoreSpaces(spaces, pos, boardState, extraConstraints);
	}
	return false;
}

std::istream& operator>>(std::istream& in, Sudoku& sudoku)
{
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			NumType num{};
			in >> num;
			sudoku.AddNum(i, j, num);
		}
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
{
	std::ostringstream os;
	os << "-> "sv << sudoku.GetName() << '\n';
	for (size_t i{}; i < ROW_SIZE; i++)
	{
		for (size_t j{}; j < COL_SIZE; j++)
		{
			os << sudoku(i, j) << " "sv;
		}
		os << '\n';
	}
	out << os.str();
	return out;
}
