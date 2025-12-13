#include "stdafx.h"

#include "sudoku.h"

Sudoku::Sudoku()
{
}

void Sudoku::AddNum(size_t i, size_t j, int Num)
{
    Board[K(i, j)] = Num;
    AddRowNum(i, j, Num);
    AddColNum(i, j, Num);
    AddSquareNum(i, j, Num);
}

void Sudoku::RemoveNum(size_t i, size_t j)
{
    int Num{Board[K(i, j)]};
    RemoveRowNum(i, j, Num);
    RemoveColNum(i, j, Num);
    RemoveSquareNum(i, j, Num);
    Board[K(i, j)] = 0;
}

bool Sudoku::Solve()
{
    InitializeExtraConstraints();
    if (!CheckOnce(FindSpaces()) || !DFS(FindSpaces()))
    {
        std::cerr << "*** 数独无解 ***" << std::endl;
        return false;
    }
    return true;
}

std::string_view Sudoku::GetName() const
{
    using std::operator""sv;
    return "数独"sv;
}

void Sudoku::InitializeExtraConstraints()
{
}

bool Sudoku::SatisfyConstraints(size_t i, size_t j, int Num) const
{
    return !RowHasNum(i, j, Num) && !ColHasNum(i, j, Num) && !SquareHasNum(i, j, Num) &&
           std::ranges::all_of(ExtraConstraints[K(i, j)],
                               [Num](const Constraint &ExtraConstraint)
                               { return ExtraConstraint(Num); });
}

int Sudoku::CalculateCandidateCount(size_t i, size_t j, int &TargetNum) const
{
    int Count{};
    for (int Num{1}; Num <= NUM_SIZE; Num++)
    {
        if (SatisfyConstraints(i, j, Num))
        {
            Count++;
            TargetNum = Num;
        }
    }
    return Count;
}

std::vector<Sudoku::Position> Sudoku::FindSpaces() const
{
    std::vector<Position> Spaces;
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            if (!Board[K(i, j)])
            {
                Spaces.emplace_back(i, j);
            }
        }
    }
    std::ranges::sort(Spaces,
                      [this](Position p1, Position p2)
                      {
                          int DevNull{};
                          return CalculateCandidateCount(p1.first, p1.second, DevNull) < CalculateCandidateCount(p2.first, p2.second, DevNull);
                      });
    return Spaces;
}

void Sudoku::RestorSpaces(const std::vector<Position> &Spaces, size_t pos)
{
    size_t n{Spaces.size()};
    for (; pos < n; pos++)
    {
        auto &&[i, j]{Spaces[pos]};
        if (Board[K(i, j)])
        {
            RemoveNum(i, j);
        }
    }
}

bool Sudoku::CheckOnce(const std::vector<Position> &Spaces)
{
    bool CheckOver{};
    while (!CheckOver)
    {
        CheckOver = true;
        for (auto &&[i, j] : Spaces)
        {
            if (Board[K(i, j)])
            {
                continue;
            }
            if (int TargetNum{}, Count{CalculateCandidateCount(i, j, TargetNum)};
                Count == 1)
            {
                AddNum(i, j, TargetNum);
                CheckOver = false;
            }
            else if (Count == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::DFS(const std::vector<Position> &Spaces, size_t pos)
{
    if (pos == Spaces.size())
    {
        return true;
    }
    auto &&[i, j]{Spaces[pos]};
    if (Board[K(i, j)])
    {
        return DFS(Spaces, pos + 1);
    }
    for (int Num{1}; Num <= NUM_SIZE; Num++)
    {
        if (!SatisfyConstraints(i, j, Num))
        {
            continue;
        }
        AddNum(i, j, Num);
        if (CheckOnce(Spaces) && DFS(Spaces, pos + 1))
        {
            return true;
        }
        RestorSpaces(Spaces, pos);
    }
    return false;
}

std::istream &operator>>(std::istream &in, Sudoku &sudoku)
{
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            int Num{};
            in >> Num;
            sudoku.AddNum(i, j, Num);
        }
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Sudoku &sudoku)
{
    out << "-> " << sudoku.GetName() << std::endl;
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            out << sudoku(i, j) << " ";
        }
        out << std::endl;
    }
    return out;
}
