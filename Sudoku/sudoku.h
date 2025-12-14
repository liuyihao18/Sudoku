#pragma once
#include "constant.h"

class Solver;

class Sudoku
{
public:
	Sudoku() = default;
	virtual ~Sudoku() = default;
	Sudoku(const Sudoku&) = default;
	Sudoku(Sudoku&&) = default;
	Sudoku& operator=(const Sudoku&) = default;
	Sudoku& operator=(Sudoku&&) = default;

	[[nodiscard]] virtual std::string_view GetName() const;
	virtual void InitializeSolver(Solver& solver);

	[[nodiscard]] bool HasNum(const size_t i, const size_t j, const NumType num) const
	{
		return RowHasNum(i, j, num) || ColHasNum(i, j, num) || SquareHasNum(i, j, num);
	}

	void AddNum(const size_t i, const size_t j, const NumType num)
	{
		Board[K(i, j)] = num;
		AddRowNum(i, j, num);
		AddColNum(i, j, num);
		AddSquareNum(i, j, num);
	}

	void RemoveNum(const size_t i, const size_t j)
	{
		RemoveRowNum(i, j);
		RemoveColNum(i, j);
		RemoveSquareNum(i, j);
		Board[K(i, j)] = 0;
	}

	NumType& operator()(const size_t i, const size_t j) { return Board[K(i, j)]; }
	const NumType& operator()(const size_t i, const size_t j) const { return Board[K(i, j)]; }
	NumType& operator[](const size_t k) { return Board[k]; }
	const NumType& operator[](const size_t k) const { return Board[k]; }

	friend std::istream& operator>>(std::istream& in, Sudoku& sudoku);
	friend std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku);

private:
	BoardType Board{};
	std::array<Bitmap, ROW_SIZE> RowNumbers{};
	std::array<Bitmap, COL_SIZE> ColNumbers{};
	std::array<Bitmap, SQUARE_ROW_SIZE * SQUARE_COL_SIZE> SquareNumbers{};

	[[nodiscard]] bool RowHasNum(const size_t i, const size_t, const NumType num) const
	{
		return RowNumbers[i] & (1 << num);
	}

	[[nodiscard]] bool ColHasNum(const size_t, const size_t j, const NumType num) const
	{
		return ColNumbers[j] & (1 << num);
	}

	[[nodiscard]] bool SquareHasNum(const size_t i, const size_t j, const NumType num) const
	{
		return SquareNumbers[SquareK(i, j)] & (1 << num);
	}

	void AddRowNum(const size_t i, const size_t, const NumType num)
	{
		RowNumbers[i] |= (1 << num);
	}

	void AddColNum(const size_t, const size_t j, const NumType num)
	{
		ColNumbers[j] |= (1 << num);
	}

	void AddSquareNum(const size_t i, const size_t j, const NumType num)
	{
		SquareNumbers[SquareK(i, j)] |= (1 << num);
	}

	void RemoveRowNum(const size_t i, const size_t j)
	{
		RowNumbers[i] &= ~(1 << Board[K(i, j)]);
	}

	void RemoveColNum(const size_t i, const size_t j)
	{
		ColNumbers[j] &= ~(1 << Board[K(i, j)]);
	}

	void RemoveSquareNum(const size_t i, const size_t j)
	{
		SquareNumbers[SquareK(i, j)] &= ~(1 << Board[K(i, j)]);
	}
};
