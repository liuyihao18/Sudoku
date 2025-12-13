#pragma once

#include "thread_pool.h"

constexpr size_t NUM_SIZE{9};
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
    using BoardType = std::array<int, BOARD_SIZE>;
    using Bitmap = int;

    BoardType Board{};
    std::array<Bitmap, ROW_SIZE> RowNums{};
    std::array<Bitmap, COL_SIZE> ColNums{};
    std::array<Bitmap, BOARD_SIZE> SquareNums{};
};

class Sudoku
{
public:
    using BoardType = FBoardState::BoardType;
    using Position = std::pair<size_t, size_t>;
    using Constraint = std::function<bool(int, const BoardType &)>;

    Sudoku();
    void AddNum(size_t i, size_t j, int Num) { AddNum(i, j, Num, _BoardState); }
    void RemoveNum(size_t i, size_t j) { RemoveNum(i, j, _BoardState); }
    bool Solve();

    virtual std::string_view GetName() const;

protected:
    std::array<std::vector<Constraint>, BOARD_SIZE> ExtraConstraints{};
    virtual void InitializeExtraConstraints();

private:
    bool SatisfyConstraints(size_t i, size_t j, int Num, const FBoardState &BoardState) const;
    int CalculateCandidateCount(size_t i, size_t j, int &TargetNum, const FBoardState &BoardState) const;
    std::vector<Position> FindSpaces(const FBoardState &BoardState) const;
    void RestorSpaces(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState);
    bool CheckOnce(const std::vector<Position> &Spaces, FBoardState &BoardState);
    bool DFS(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState);

private:
    ThreadPool _ThreadPool{8};
    bool ThreadDFS(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState);

protected:
    static size_t K(size_t i, size_t j) { return i * COL_SIZE + j; }
    static size_t I(size_t k) { return k / COL_SIZE; }
    static size_t J(size_t k) { return k % COL_SIZE; }
    static size_t SquareK(size_t i, size_t j) { return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE; }

private:
    FBoardState _BoardState{};
    static bool RowHasNum(size_t i, size_t j, int Num, const FBoardState &BoardState) { return BoardState.RowNums[i] & (1 << Num); }
    static bool ColHasNum(size_t i, size_t j, int Num, const FBoardState &BoardState) { return BoardState.ColNums[j] & (1 << Num); }
    static bool SquareHasNum(size_t i, size_t j, int Num, const FBoardState &BoardState) { return BoardState.SquareNums[SquareK(i, j)] & (1 << Num); }
    static bool HasNum(size_t i, size_t j, int Num, const FBoardState &BoardState)
    {
        return RowHasNum(i, j, Num, BoardState) || ColHasNum(i, j, Num, BoardState) || SquareHasNum(i, j, Num, BoardState);
    }
    static void AddRowNum(size_t i, size_t j, int Num, FBoardState &BoardState) { BoardState.RowNums[i] |= (1 << Num); }
    static void AddColNum(size_t i, size_t j, int Num, FBoardState &BoardState) { BoardState.ColNums[j] |= (1 << Num); }
    static void AddSquareNum(size_t i, size_t j, int Num, FBoardState &BoardState) { BoardState.SquareNums[SquareK(i, j)] |= (1 << Num); }
    static void AddNum(size_t i, size_t j, int Num, FBoardState &BoardState)
    {
        BoardState.Board[K(i, j)] = Num;
        AddRowNum(i, j, Num, BoardState);
        AddColNum(i, j, Num, BoardState);
        AddSquareNum(i, j, Num, BoardState);
    }
    static void RemoveRowNum(size_t i, size_t j, FBoardState &BoardState) { BoardState.RowNums[i] &= ~(1 << BoardState.Board[K(i, j)]); }
    static void RemoveColNum(size_t i, size_t j, FBoardState &BoardState) { BoardState.ColNums[j] &= ~(1 << BoardState.Board[K(i, j)]); }
    static void RemoveSquareNum(size_t i, size_t j, FBoardState &BoardState) { BoardState.SquareNums[SquareK(i, j)] &= ~(1 << BoardState.Board[K(i, j)]); }
    static void RemoveNum(size_t i, size_t j, FBoardState &BoardState)
    {
        RemoveRowNum(i, j, BoardState);
        RemoveColNum(i, j, BoardState);
        RemoveSquareNum(i, j, BoardState);
        BoardState.Board[K(i, j)] = 0;
    }

public:
    int &operator()(size_t i, size_t j) { return _BoardState.Board[K(i, j)]; }
    const int &operator()(size_t i, size_t j) const { return _BoardState.Board[K(i, j)]; }
    int &operator[](size_t k) { return _BoardState.Board[k]; }
    const int &operator[](size_t k) const { return _BoardState.Board[k]; }
    int &Board(size_t i, size_t j) { return _BoardState.Board[K(i, j)]; }
    const int &Board(size_t i, size_t j) const { return _BoardState.Board[K(i, j)]; }
    friend std::istream &operator>>(std::istream &in, Sudoku &sudoku);
    friend std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku);
};
