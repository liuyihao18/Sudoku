#include "stdafx.h"

#include "continuous_sudoku.h"

static void check(size_t i1, size_t j1, size_t i2, size_t j2)
{
    int _i1{static_cast<int>(i1)};
    int _j1{static_cast<int>(j1)};
    int _i2{static_cast<int>(i2)};
    int _j2{static_cast<int>(j2)};
    if (!((abs(_i2 - _i1) == 0 && abs(_j2 - _j1) == 1) || (abs(_i2 - _i1) == 1 && abs(_j2 - _j1) == 0)))
    {
        std::ostringstream os;
        os << "- 连续数独约束错误：("sv << _i1 + 1 << ", "sv << _j1 + 1 << ") <-> ("sv << _i2 + 1 << ", "sv << _j2 + 1 << ") 不连续"sv << std::endl;
        std::cerr << os.str();
        exit(1);
    }
}

std::string_view ContinuousSudoku::GetName() const
{
    return "连续数独"sv;
}

void ContinuousSudoku::InitializeExtraConstraints()
{
    for (const std::vector<Position> &ContinuousPosition : ContinuousPositions)
    {
        auto &&[i1, j1]{ContinuousPosition[0]};
        auto &&[i2, j2]{ContinuousPosition[1]};
        check(i1, j1, i2, j2);
        Constraint ContinuousConstrain1{
            [i2, j2, this](int Num, const BoardType &Board)
            {
                return !Board[K(i2, j2)] || Board[K(i2, j2)] - Num == 1 || Num - Board[K(i2, j2)] == 1;
            }};
        Constraint ContinuousConstraint2{
            [i1, j1, this](int Num, const BoardType &Board)
            {
                return !Board[K(i1, j1)] || Board[K(i1, j1)] - Num == 1 || Num - Board[K(i1, j1)] == 1;
            }};
        ExtraConstraints[K(i1, j1)].emplace_back(std::move(ContinuousConstrain1));
        ExtraConstraints[K(i2, j2)].emplace_back(std::move(ContinuousConstraint2));
    }
}

std::istream &operator>>(std::istream &in, ContinuousSudoku &sudoku)
{
    in >> static_cast<Sudoku &>(sudoku);

    /**
     * 连续数独额外约束输入格式：
     *   - 每一行一条连续约束
     *   - x1 y1 x2 y2
     *   - 满足 |Board(x1, y1) - Board(x2, y2)| = 1
     */
    std::string line{};
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::istringstream iss(line);
        std::vector<Sudoku::Position> ContinuousPosition;
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
                ContinuousPosition.emplace_back(std::move(p));
                continue;
            }
            std::ostringstream os;
            os << "- 连续数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立"sv << std::endl;
            std::cerr << os.str();
            exit(1);
        }
        if (ContinuousPosition.size() > 2)
        {
            std::ostringstream os;
            os << "- 连续数独约束错误："sv << ContinuousPosition.size() << " > 2"sv << std::endl;
            std::cerr << os.str();
            exit(1);
        }
        sudoku.ContinuousPositions.emplace_back(std::move(ContinuousPosition));
    }
    return in;
}
