#pragma once
#include <iostream>
#include <vector>
#include <functional>

constexpr size_t NUM_SIZE{9};
constexpr size_t ROW_SIZE{9};
constexpr size_t COL_SIZE{9};
constexpr size_t SQUARE_SIZE{3};
constexpr size_t SQUARE_ROW_SIZE{ROW_SIZE / SQUARE_SIZE};
constexpr size_t SQUARE_COL_SIZE{COL_SIZE / SQUARE_SIZE};

static_assert(ROW_SIZE == NUM_SIZE);
static_assert(COL_SIZE == NUM_SIZE);
static_assert(ROW_SIZE == SQUARE_SIZE * SQUARE_ROW_SIZE);
static_assert(COL_SIZE == SQUARE_SIZE * SQUARE_COL_SIZE);
static_assert(SQUARE_SIZE * SQUARE_SIZE == NUM_SIZE);

class Sudoku
{
public:
    using Position = std::pair<size_t, size_t>;
    using Constraint = std::function<bool(int)>;

    Sudoku();
    void AddNum(size_t i, size_t j, int Num);
    void RemoveNum(size_t i, size_t j);
    bool Solve();

    virtual std::string_view GetName() const = 0;

protected:
    std::vector<std::vector<Constraint>> ExtraConstraints;
    virtual void InitializeExtraConstraints();

private:
    bool SatisfyConstraints(size_t i, size_t j, int Num) const;
    int CalculateCandidateCount(size_t i, size_t j, int &TargetNum) const;
    std::vector<Position> FindSpaces() const;
    void RestorSpaces(const std::vector<Position> &Spaces, size_t pos);
    bool CheckOnce(const std::vector<Position> &Spaces);
    bool DFS(const std::vector<Position> &Spaces, size_t pos = 0);

protected:
    std::vector<int> Board;
    static size_t K(size_t i, size_t j) { return i * COL_SIZE + j; }
    static size_t I(size_t k) { return k / COL_SIZE; }
    static size_t J(size_t k) { return k % COL_SIZE; }
    static size_t SquareK(size_t i, size_t j) { return i / SQUARE_SIZE * SQUARE_COL_SIZE + j / SQUARE_SIZE; }

private:
    std::vector<int> RowNums;
    std::vector<int> ColNums;
    std::vector<int> SquareNums;
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
