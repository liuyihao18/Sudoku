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
	virtual bool SatisfyConstraints(size_t i, size_t j, int num);
	virtual void AddConstraints(size_t i, size_t j, int num);
	virtual void RemoveConstraints(size_t i, size_t j, int num);

protected:
	void InitializeConstraints();
	bool CheckOnce(const std::vector<Position> &spaces);
	bool DFS(const std::vector<Position> &spaces, size_t pos = 0);
	int GetCandidateCount(size_t i, size_t j, int &TargetNum);
	std::vector<Position> GetSpaces();

protected:
	std::vector<int> Board;
	std::vector<int> RowConstraints;
	std::vector<int> ColConstraints;
	std::vector<int> SquareConstraints;
	inline size_t K(size_t i, size_t j) const { return i * COL_SIZE + j; }
	inline size_t I(size_t k) const { return k / COL_SIZE; }
	inline size_t J(size_t k) const { return k % COL_SIZE; }
	inline size_t SquareK(size_t i, size_t j) const;
	bool CheckRowConstraints(size_t i, size_t j, int num);
	bool CheckColConstraints(size_t i, size_t j, int num);
	bool CheckSquareConstraints(size_t i, size_t j, int num);
	void AddRowConstraints(size_t i, size_t j, int num);
	void AddColConstraints(size_t i, size_t j, int num);
	void AddSquareConstraints(size_t i, size_t j, int num);
	void RemoveRowConstraints(size_t i, size_t j, int num);
	void RemoveColConstraints(size_t i, size_t j, int num);
	void RemoveSquareConstraints(size_t i, size_t j, int num);

public:
	int &operator()(size_t i, size_t j);
	const int &operator()(size_t i, size_t j) const;
	int &operator[](size_t k);
	const int &operator[](size_t k) const;
	friend std::istream &operator>>(std::istream &in, Sudoku &sudoku);
	friend std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku);
};

inline size_t Sudoku::SquareK(size_t i, size_t j) const
{
	size_t SquareI = i / SQUARE_SIZE;
	size_t SquareJ = j / SQUARE_SIZE;
	size_t SquareK = SquareI * SQUARE_COL_SIZE + SquareJ;
	return SquareK;
}
