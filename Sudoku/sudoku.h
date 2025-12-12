#pragma once
#include <iostream>
#include <vector>
#include <functional>

constexpr size_t NUM_SIZE = 9;
constexpr size_t ROW_SIZE = 9;
constexpr size_t COL_SIZE = 9;
constexpr size_t SQUARE_SIZE = 3;
constexpr size_t SQUARE_ROW_SIZE = ROW_SIZE / SQUARE_SIZE;
constexpr size_t SQUARE_COL_SIZE = COL_SIZE / SQUARE_SIZE;

class Sudoku
{
public:
	using Position = std::pair<size_t, size_t>;
	using Constraint = std::function<bool(int)>;

	Sudoku();
	bool Solve();

protected:
	virtual void InitializeExtraConstraints();

protected:
	bool CheckOnce(const std::vector<Position> &Spaces);
	bool DFS(const std::vector<Position> &Spaces, size_t pos = 0);
	bool SatisfyConstraints(size_t i, size_t j, int Num);
	void AddNum(size_t i, size_t j, int Num);
	void RemoveNum(size_t i, size_t j);
	int GetCandidateCount(size_t i, size_t j, int &TargetNum);
	std::vector<Position> GetSpaces();
	void RestorSpaces(const std::vector<Position> &Spaces, size_t pos);

protected:
	std::vector<int> Board;
	std::vector<int> RowNums;
	std::vector<int> ColNums;
	std::vector<int> SquareNums;
	std::vector<std::vector<Constraint>> ExtraConstraints;
	size_t K(size_t i, size_t j) const { return i * COL_SIZE + j; }
	size_t I(size_t k) const { return k / COL_SIZE; }
	size_t J(size_t k) const { return k % COL_SIZE; }
	size_t SquareK(size_t i, size_t j) const { return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE; }
	bool RowHasNum(size_t i, size_t j, int Num) const { return RowNums[i] & (1 << Num); }
	bool ColHasNum(size_t i, size_t j, int Num) const { return ColNums[j] & (1 << Num); }
	bool SquareHasNum(size_t i, size_t j, int Num) const { return SquareNums[SquareK(i, j)] & (1 << Num); }
	void AddRowNum(size_t i, size_t j, int Num) { RowNums[i] |= (1 << Num); }
	void AddColNum(size_t i, size_t j, int Num) { ColNums[j] |= (1 << Num); }
	void AddSquareNum(size_t i, size_t j, int Num) { SquareNums[SquareK(i, j)] |= (1 << Num); }
	void RemoveRowNum(size_t i, size_t j, int Num) { RowNums[i] &= ~(1 << Num); }
	void RemoveColNum(size_t i, size_t j, int Num) { ColNums[j] &= ~(1 << Num); }
	void RemoveSquareNum(size_t i, size_t j, int Num) { SquareNums[SquareK(i, j)] &= ~(1 << Num); }

public:
	int &operator()(size_t i, size_t j) { return Board[K(i, j)]; }
	const int &operator()(size_t i, size_t j) const { return Board[K(i, j)]; }
	int &operator[](size_t k) { return Board[k]; }
	const int &operator[](size_t k) const { return Board[k]; }
	friend std::istream &operator>>(std::istream &in, Sudoku &sudoku);
	friend std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku);
};
