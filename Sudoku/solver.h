#pragma once
#include "constant.h"

class Sudoku;

class Solver
{
public:
	static bool Solve(Sudoku& sudoku);

private:
	static size_t CalculateCandidateCount(const Sudoku& sudoku, size_t i, size_t j, NumType& targetNum);
	static bool CheckOnce(Sudoku& sudoku, const std::vector<Position>& spaces, size_t pos);
	static void RestoreSpaces(Sudoku& sudoku, const std::vector<Position>& spaces, size_t pos);
	static bool Dfs(Sudoku& sudoku, const std::vector<Position>& spaces, size_t pos,
	                const std::shared_ptr<std::atomic<bool>>& solved);
};
