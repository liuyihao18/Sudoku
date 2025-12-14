#pragma once

using NumType = int32_t;

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

struct FBoardState
{
	using BoardType = std::array<NumType, BOARD_SIZE>;
	using Bitmap = size_t;

	BoardType Board{};
	std::array<Bitmap, ROW_SIZE> RowNumbers{};
	std::array<Bitmap, COL_SIZE> ColNumbers{};
	std::array<Bitmap, SQUARE_ROW_SIZE * SQUARE_COL_SIZE> SquareNumbers{};
};

class Sudoku
{
public:
	using BoardType = FBoardState::BoardType;
	using Position = std::pair<size_t, size_t>;
	using Constraint = std::function<bool(NumType, const BoardType&)>;
	using ExtraConstraintsType = std::array<std::vector<Constraint>, BOARD_SIZE>;

	Sudoku();
	virtual ~Sudoku() = default;
	Sudoku(const Sudoku&) = default;
	Sudoku(Sudoku&&) = default;
	Sudoku& operator=(const Sudoku&) = default;
	Sudoku& operator=(Sudoku&&) = default;

	void AddNum(const size_t i, const size_t j, const NumType num) { AddNum(i, j, num, BoardState); }
	void RemoveNum(const size_t i, const size_t j) { RemoveNum(i, j, BoardState); }
	bool Solve();

	[[nodiscard]] virtual std::string_view GetName() const;

	NumType& operator()(const size_t i, const size_t j) { return BoardState.Board[K(i, j)]; }
	const NumType& operator()(const size_t i, const size_t j) const { return BoardState.Board[K(i, j)]; }
	NumType& operator[](const size_t k) { return BoardState.Board[k]; }
	const NumType& operator[](const size_t k) const { return BoardState.Board[k]; }
	friend std::istream& operator>>(std::istream& in, Sudoku& sudoku);
	friend std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku);

protected:
	std::shared_ptr<ExtraConstraintsType> ExtraConstraints{};
	virtual void InitializeExtraConstraints();

	FBoardState BoardState{};
	bool ThreadDfs();

	static size_t K(const size_t i, const size_t j) { return i * COL_SIZE + j; }
	static size_t I(const size_t k) { return k / COL_SIZE; }
	static size_t J(const size_t k) { return k % COL_SIZE; }

	static size_t SquareK(const size_t i, const size_t j)
	{
		return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE;
	}

private:
	static bool RowHasNum(const size_t i, const size_t, const NumType num, const FBoardState& boardState)
	{
		return boardState.RowNumbers[i] & (1z << num);
	}

	static bool ColHasNum(const size_t, const size_t j, const NumType num, const FBoardState& boardState)
	{
		return boardState.ColNumbers[j] & (1z << num);
	}

	static bool SquareHasNum(const size_t i, const size_t j, const NumType num, const FBoardState& boardState)
	{
		return boardState.SquareNumbers[SquareK(i, j)] & (1z << num);
	}

	static bool HasNum(const size_t i, const size_t j, const NumType num, const FBoardState& boardState)
	{
		return RowHasNum(i, j, num, boardState) || ColHasNum(i, j, num, boardState) || SquareHasNum(
			i, j, num, boardState);
	}

	static void AddRowNum(const size_t i, const size_t, const NumType num, FBoardState& boardState)
	{
		boardState.RowNumbers[i] |= (1z << num);
	}

	static void AddColNum(const size_t, const size_t j, const NumType num, FBoardState& boardState)
	{
		boardState.ColNumbers[j] |= (1z << num);
	}

	static void AddSquareNum(const size_t i, const size_t j, const NumType num, FBoardState& boardState)
	{
		boardState.SquareNumbers[SquareK(i, j)] |= (1z << num);
	}

	static void AddNum(const size_t i, const size_t j, const NumType num, FBoardState& boardState)
	{
		boardState.Board[K(i, j)] = num;
		AddRowNum(i, j, num, boardState);
		AddColNum(i, j, num, boardState);
		AddSquareNum(i, j, num, boardState);
	}

	static void RemoveRowNum(const size_t i, const size_t j, FBoardState& boardState)
	{
		boardState.RowNumbers[i] &= ~(1 << boardState.Board[K(i, j)]);
	}

	static void RemoveColNum(const size_t i, const size_t j, FBoardState& boardState)
	{
		boardState.ColNumbers[j] &= ~(1 << boardState.Board[K(i, j)]);
	}

	static void RemoveSquareNum(const size_t i, const size_t j, FBoardState& boardState)
	{
		boardState.SquareNumbers[SquareK(i, j)] &= ~(1 << boardState.Board[K(i, j)]);
	}

	static void RemoveNum(const size_t i, const size_t j, FBoardState& boardState)
	{
		RemoveRowNum(i, j, boardState);
		RemoveColNum(i, j, boardState);
		RemoveSquareNum(i, j, boardState);
		boardState.Board[K(i, j)] = 0;
	}

	static bool SatisfyConstraints(size_t i, size_t j, NumType num, const FBoardState& boardState,
	                               const ExtraConstraintsType& extraConstraints);
	static size_t CalculateCandidateCount(size_t i, size_t j, NumType& targetNum, const FBoardState& boardState,
	                                      const ExtraConstraintsType& extraConstraints);
	static std::vector<Position> FindSpaces(const FBoardState& boardState,
	                                        const ExtraConstraintsType& extraConstraints);
	static void RestoreSpaces(const std::vector<Position>& spaces, size_t pos, FBoardState& boardState,
	                          const ExtraConstraintsType& extraConstraints);
	static bool CheckOnce(const std::vector<Position>& spaces, FBoardState& boardState,
	                      const ExtraConstraintsType& extraConstraints);
	static bool Dfs(const std::vector<Position>& spaces, size_t pos, FBoardState& boardState,
	                const ExtraConstraintsType& extraConstraints);
};
