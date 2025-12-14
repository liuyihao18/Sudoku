#pragma once
#include "constant.h"

class Sudoku;

class Solver
{
public:
	Solver();
	bool Solve(Sudoku& sudoku) const;

	template <typename Func>
	void AddConstraint(size_t i, size_t j, Func&& func);

private:
	std::shared_ptr<ConstraintsType> ExtraConstraints{};

	static bool SatisfyConstraints(size_t i, size_t j, NumType num, const Sudoku& sudoku,
	                               const ConstraintsType& extraConstraints);
	static size_t CalculateCandidateCount(size_t i, size_t j, NumType& targetNum, const Sudoku& sudoku,
	                                      const ConstraintsType& extraConstraints);
	static std::vector<Position> FindSpaces(const Sudoku& sudoku,
	                                        const ConstraintsType& extraConstraints);
	static void RestoreSpaces(const std::vector<Position>& spaces, size_t pos, Sudoku& sudoku,
	                          const ConstraintsType& extraConstraints);
	static bool CheckOnce(const std::vector<Position>& spaces, Sudoku& sudoku,
	                      const ConstraintsType& extraConstraints);
	static bool Dfs(const std::vector<Position>& spaces, size_t pos, Sudoku& sudoku,
	                const ConstraintsType& extraConstraints);
};

template <typename Func>
void Solver::AddConstraint(const size_t i, const size_t j, Func&& func)
{
	(*ExtraConstraints)[K(i, j)].emplace_back(std::forward<Func>(func));
}
