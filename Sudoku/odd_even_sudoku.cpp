#include "stdafx.h"

#include "odd_even_sudoku.h"

std::string_view OddEvenSudoku::GetName() const
{
    return "奇偶数独"sv;
}

void OddEvenSudoku::InitializeExtraConstraints()
{
    for (auto &&[i, j] : Odd)
    {
        Constraint OddConstraint{
            [this](int Num, const BoardType &Board)
            {
                return Num & 1;
            }};
        (*ExtraConstraints)[K(i, j)].emplace_back(std::move(OddConstraint));
    }
    for (auto &&[i, j] : Even)
    {
        Constraint EvenConstraint{
            [this](int Num, const BoardType &Board)
            {
                return !(Num & 1);
            }};
        (*ExtraConstraints)[K(i, j)].emplace_back(std::move(EvenConstraint));
    }
}

std::istream &operator>>(std::istream &in, OddEvenSudoku &sudoku)
{
    in >> static_cast<Sudoku &>(sudoku);

    /**
     * 奇偶数独额外约束输入格式：
     *   - 第一行为奇数位置：x1 y1 x2 y2 ...
     *   - 第二行为偶数位置：x1 y1 x2 y2 ...
     */
    auto Input{
        [&in](std::vector<Sudoku::Position> &Positions)
        {
            std::string line{};
            while (line.empty())
            {
                std::getline(in, line);
            }
            std::istringstream iss(line);
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
                os << "- 奇偶数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立"sv << std::endl;
                std::cerr << os.str();
                exit(1);
            }
        }};
    Input(sudoku.Odd);
    Input(sudoku.Even);
    return in;
}
