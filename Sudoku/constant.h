#pragma once

using NumType = int16_t;
using Bitmap = uint16_t;
using Position = std::pair<size_t, size_t>;

constexpr NumType NUM_SIZE{9};
constexpr size_t ROW_SIZE{9};
constexpr size_t COL_SIZE{9};
constexpr size_t SQUARE_SIZE{3};
constexpr size_t SQUARE_ROW_SIZE{ROW_SIZE / SQUARE_SIZE};
constexpr size_t SQUARE_COL_SIZE{COL_SIZE / SQUARE_SIZE};
constexpr size_t BOARD_SIZE{ROW_SIZE * COL_SIZE};

static_assert(ROW_SIZE == NUM_SIZE);
static_assert(COL_SIZE == NUM_SIZE);
static_assert(ROW_SIZE == SQUARE_SIZE * SQUARE_ROW_SIZE);
static_assert(COL_SIZE == SQUARE_SIZE * SQUARE_COL_SIZE);
static_assert(SQUARE_SIZE * SQUARE_SIZE == NUM_SIZE);

using BoardType = std::array<NumType, BOARD_SIZE>;
using Constraint = std::function<bool(NumType, const class Sudoku&)>;
using ConstraintsType = std::array<std::vector<Constraint>, BOARD_SIZE>;

inline size_t K(const size_t i, const size_t j) { return i * COL_SIZE + j; }
inline size_t I(const size_t k) { return k / COL_SIZE; }
inline size_t J(const size_t k) { return k % COL_SIZE; }

inline size_t SquareK(const size_t i, const size_t j)
{
	return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE;
}
