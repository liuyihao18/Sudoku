#include "stdafx.h"

#include "killer_sudoku.h"

static void check(const std::vector<Sudoku::Position> &Positions)
{
    auto Distance{
        [](const Sudoku::Position &p1, const Sudoku::Position &p2)
        {
            size_t DeltaI{p1.first <= p2.first ? p2.first - p1.first : p1.first - p2.first};
            size_t DeltaJ{p1.second <= p2.second ? p2.second - p1.second : p1.second - p2.second};
            return DeltaI + DeltaJ;
        }};
    size_t n{Positions.size()};
    for (size_t i{}; i < n; i++)
    {
        size_t MinDistance{0xffffffff};
        Sudoku::Position p1{Positions[i]};
        for (size_t j{}; j < n; j++)
        {
            if (j == i)
            {
                continue;
            }
            Sudoku::Position p2{Positions[j]};
            MinDistance = std::min(MinDistance, Distance(p1, p2));
        }
        if (MinDistance != 1)
        {
            std::ostringstream os;
            os << "- 杀手数独约束错误：("sv << p1.first + 1 << ", "sv << p1.second + 1 << ") 不连续"sv << std::endl;
            std::cerr << os.str();
            exit(1);
        }
    }
}

std::string_view KillerSudoku::GetName() const
{
    return "杀手数独"sv;
}

void KillerSudoku::InitializeExtraConstraints()
{
    for (const Killer &Killer : Killers)
    {
        check(Killer.Positions);
        for (auto &&[i, j] : Killer.Positions)
        {
            Constraint KillerConstraint{
                [Killer, i, j, this](int Num, const BoardType &Board)
                {
                    bool HasZero{false};
                    int Temp{0};
                    for (auto &&[ii, jj] : Killer.Positions)
                    {
                        if (ii == i && jj == j)
                        {
                            continue;
                        }
                        HasZero |= Board[K(ii, jj)] == 0;
                        Temp += Board[K(ii, jj)];
                    }
                    return HasZero ? Temp + Num <= Killer.Sum : Temp + Num == Killer.Sum;
                }};
            (*ExtraConstraints)[K(i, j)].emplace_back(std::move(KillerConstraint));
        }
    }
}

std::istream &operator>>(std::istream &in, KillerSudoku &sudoku)
{
    in >> static_cast<Sudoku &>(sudoku);

    /**
     * 杀手数独额外约束输入格式：
     *   - 每一行一个范围和
     *   - sum x1 y1 x2 y2 ...
     *   - 满足 sum = Board(x1, y1) + Board(x2, y2) + ...
     */
    std::string line{};
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::istringstream iss(line);
        int Sum{};
        std::vector<Sudoku::Position> Positions;
        iss >> Sum;
        while (iss)
        {
            Sudoku::Position p{};
            iss >> p.first >> p.second;
            if (p.first == 0 && p.second == 0)
            {
                break;
            }
            if (p.first != 0 && p.second != 0)
            {
                p.first--;
                p.second--;
                Positions.emplace_back(std::move(p));
                continue;
            }
            std::ostringstream os;
            os << "- 杀手数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立"sv << std::endl;
            std::cerr << os.str();
            exit(1);
        }
        sudoku.Killers.emplace_back(Sum, std::move(Positions));
    }
    return in;
}
