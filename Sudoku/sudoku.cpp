#include "stdafx.h"

#include "sudoku.h"

Sudoku::Sudoku()
{
}

bool Sudoku::Solve()
{
    InitializeExtraConstraints();
    if (!CheckOnce(FindSpaces(_BoardState), _BoardState) || !DFS(FindSpaces(_BoardState), 0, _BoardState))
    {
        std::cerr << "*** 数独无解 ***"sv << std::endl;
        return false;
    }
    return true;
}

std::string_view Sudoku::GetName() const
{
    return "数独"sv;
}

void Sudoku::InitializeExtraConstraints()
{
}

bool Sudoku::SatisfyConstraints(size_t i, size_t j, int Num, const FBoardState &BoardState) const
{
    return !HasNum(i, j, Num, BoardState) &&
           std::ranges::all_of(ExtraConstraints[K(i, j)],
                               [Num, &BoardState](const Constraint &ExtraConstraint)
                               { return ExtraConstraint(Num, BoardState.Board); });
}

int Sudoku::CalculateCandidateCount(size_t i, size_t j, int &TargetNum, const FBoardState &BoardState) const
{
    int Count{};
    for (int Num{1}; Num <= NUM_SIZE; Num++)
    {
        if (SatisfyConstraints(i, j, Num, BoardState))
        {
            Count++;
            TargetNum = Num;
        }
    }
    return Count;
}

std::vector<Sudoku::Position> Sudoku::FindSpaces(const FBoardState &BoardState) const
{
    std::vector<Position> Spaces;
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            if (!BoardState.Board[K(i, j)])
            {
                Spaces.emplace_back(i, j);
            }
        }
    }
    /*
    std::ranges::sort(Spaces,
                      [this, &BoardState](const Position &p1, const Position &p2)
                      {
                          int DevNull = 0;
                          return CalculateCandidateCount(p1.first, p1.second, DevNull, BoardState) < CalculateCandidateCount(p2.first, p2.second, DevNull, BoardState);
                      });
    */
    return Spaces;
}

void Sudoku::RestorSpaces(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState)
{
    size_t n{Spaces.size()};
    for (; pos < n; pos++)
    {
        auto &&[i, j]{Spaces[pos]};
        if (BoardState.Board[K(i, j)])
        {
            RemoveNum(i, j, BoardState);
        }
    }
}

bool Sudoku::CheckOnce(const std::vector<Position> &Spaces, FBoardState &BoardState)
{
    bool CheckOver{};
    while (!CheckOver)
    {
        CheckOver = true;
        for (auto &&[i, j] : Spaces)
        {
            if (BoardState.Board[K(i, j)])
            {
                continue;
            }
            if (int TargetNum{}, Count{CalculateCandidateCount(i, j, TargetNum, BoardState)};
                Count == 1)
            {
                AddNum(i, j, TargetNum, BoardState);
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

bool Sudoku::DFS(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState)
{
    if (pos == Spaces.size())
    {
        return true;
    }
    auto &&[i, j]{Spaces[pos]};
    if (BoardState.Board[K(i, j)])
    {
        return DFS(Spaces, pos + 1, BoardState);
    }
    for (int Num{1}; Num <= NUM_SIZE; Num++)
    {
        if (!SatisfyConstraints(i, j, Num, BoardState))
        {
            continue;
        }
        AddNum(i, j, Num, BoardState);
        if (CheckOnce(Spaces, BoardState) && DFS(Spaces, pos + 1, BoardState))
        {
            return true;
        }
        RestorSpaces(Spaces, pos, BoardState);
    }
    return false;
}

bool Sudoku::ThreadDFS(const std::vector<Position> &Spaces, size_t pos, FBoardState &BoardState)
{
    if (pos == Spaces.size())
    {
        return true;
    }
    auto &&[i, j] = Spaces[0];
    std::shared_ptr CopySpaces = std::make_shared<std::vector<Position>>(Spaces);
    std::vector<std::pair<std::future<bool>, std::shared_ptr<FBoardState>>> Results;
    for (int Num{1}; Num <= NUM_SIZE; Num++)
    {
        if (!SatisfyConstraints(i, j, Num, BoardState))
        {
            continue;
        }
        std::shared_ptr CopyState = std::make_shared<FBoardState>(BoardState);
        AddNum(i, j, Num, *CopyState);
        Results.emplace_back(
            _ThreadPool.AddTask(
                [CopySpaces, CopyState, this]
                {
                    return DFS(*CopySpaces, 1, *CopyState);
                }),
            CopyState);
    }
    while (std::ranges::any_of(Results,
                               [](const std::pair<std::future<bool>, std::shared_ptr<FBoardState>> &Result)
                               {
                                   return Result.first.valid();
                               }))
    {
        for (auto &&Result : Results)
        {
            if (!Result.first.valid())
            {
                continue;
            }
            std::future_status Status = Result.first.wait_for(std::chrono::milliseconds(1));
            if (Status == std::future_status::ready && Result.first.get())
            {
                BoardState = std::move(*Result.second);
                return true;
            }
        }
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
    std::ostringstream os;
    os << "-> "sv << sudoku.GetName() << std::endl;
    for (size_t i{}; i < ROW_SIZE; i++)
    {
        for (size_t j{}; j < COL_SIZE; j++)
        {
            os << sudoku(i, j) << " "sv;
        }
        os << std::endl;
    }
    out << os.str();
    return out;
}
