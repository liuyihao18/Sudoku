#pragma once
#include <iostream>
#include <vector>
#include <set>

constexpr int NUM_SIZE = 9;
constexpr int ROW_SIZE = 9;
constexpr int COL_SIZE = 9;
constexpr int INDEX_SIZE = 3;
constexpr int ROW_INDEX = ROW_SIZE / INDEX_SIZE;
constexpr int COL_INDEX = COL_SIZE / INDEX_SIZE;

class Sudoku
{
public:
	Sudoku();
	int &operator()(size_t i, size_t j);
	const int &operator()(size_t i, size_t j) const;
	bool solve();

protected:
	std::vector<int> _candidates;
	virtual bool _solve(std::vector<int> &board, std::vector<int> &candidates);
	virtual void _updateCandidates(size_t i, size_t j, int num, const std::vector<int> &board, std::vector<int> &candidates);
	inline size_t k(size_t i, size_t j) const { return i * COL_SIZE + j; }

private:
	std::vector<int> _board;
	void _initializeCandidates();
	bool _isOneCandidate(int candidates);
	int _getOneCandidate(int candidates);
	std::vector<int> _getAllCandidates(int candidates);

public:
	friend std::istream &operator>>(std::istream &in, Sudoku &sudoku);
	friend std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku);
};
