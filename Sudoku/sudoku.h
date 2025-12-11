#pragma once
#include <iostream>
#include <vector>

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

	Sudoku();
	bool Solve();

protected:
	virtual bool SatisfyConstraints(size_t i, size_t j, int Num);
	virtual void AddConstraints(size_t i, size_t j, int Num);
	virtual void RemoveConstraints(size_t i, size_t j, int Num);

protected:
	void InitializeConstraints();
	bool CheckOnce(const std::vector<Position> &Spaces);
	bool DFS(const std::vector<Position> &Spaces, size_t Pos = 0);
	int GetCandidateCount(size_t i, size_t j, int &TargetNum);
	std::vector<Position> GetSpaces();
	void RestorSpaces(const std::vector<Position> &Spaces, size_t Pos);

protected:
	std::vector<int> Board;
	std::vector<int> RowConstraints;
	std::vector<int> ColConstraints;
	std::vector<int> SquareConstraints;
	size_t K(size_t i, size_t j) const { return i * COL_SIZE + j; }
	size_t I(size_t k) const { return k / COL_SIZE; }
	size_t J(size_t k) const { return k % COL_SIZE; }
	size_t SquareK(size_t i, size_t j) const { return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE; }
	bool CheckRowConstraints(size_t i, size_t j, int Num) const { return RowConstraints[i] & (1 << Num); }
	bool CheckColConstraints(size_t i, size_t j, int Num) const { return ColConstraints[j] & (1 << Num); }
	bool CheckSquareConstraints(size_t i, size_t j, int Num) const { return SquareConstraints[SquareK(i, j)] & (1 << Num); }
	void AddRowConstraints(size_t i, size_t j, int Num) { RowConstraints[i] |= (1 << Num); }
	void AddColConstraints(size_t i, size_t j, int Num) { ColConstraints[j] |= (1 << Num); }
	void AddSquareConstraints(size_t i, size_t j, int Num) { SquareConstraints[SquareK(i, j)] |= (1 << Num); }
	void RemoveRowConstraints(size_t i, size_t j, int Num) { RowConstraints[i] &= ~(1 << Num); }
	void RemoveColConstraints(size_t i, size_t j, int Num) { ColConstraints[j] &= ~(1 << Num); }
	void RemoveSquareConstraints(size_t i, size_t j, int Num) { SquareConstraints[SquareK(i, j)] &= ~(1 << Num); }

public:
	int &operator()(size_t i, size_t j) { return Board[K(i, j)]; }
	const int &operator()(size_t i, size_t j) const { return Board[K(i, j)]; }
	int &operator[](size_t k) { return Board[k]; }
	const int &operator[](size_t k) const { return Board[k]; }
	friend std::istream &operator>>(std::istream &in, Sudoku &sudoku);
	friend std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku);
};
