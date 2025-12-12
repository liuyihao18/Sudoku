#include "thermometer_sudoku.h"
#include <string>
#include <sstream>

static void check(size_t i1, size_t j1, size_t i2, size_t j2)
{
    int _i1{static_cast<int>(i1)};
    int _j1{static_cast<int>(j1)};
    int _i2{static_cast<int>(i2)};
    int _j2{static_cast<int>(j2)};
    if (abs(_i2 - _i1) > 1 || abs(_j2 - _j1) > 1)
    {
        std::cerr << "- 温度计约束错误：(" << _i1 + 1 << ", " << _j1 + 1 << ") < (" << _i2 + 1 << ", " << _j2 + 1 << ")" << std::endl;
        exit(1);
    }
}

std::string_view ThermometerSudoku::GetName() const
{
    using std::string_view_literals::operator""sv;
    return "温度计数独"sv;
}

void ThermometerSudoku::InitializeExtraConstraints()
{
    Super::InitializeExtraConstraints();

    for (const std::vector<Position> &Thermometer : Thermometers)
    {
        size_t n{Thermometer.size()};
        for (size_t m{}; m + 1 < n; m++)
        {
            auto &&[i1, j1]{Thermometer[m]};
            auto &&[i2, j2]{Thermometer[m + 1]};
            check(i1, j1, i2, j2);
            Constraint ThermometerConstraint1{
                [i2, j2, this](int Num)
                {
                    return !Board[K(i2, j2)] || Num < Board[K(i2, j2)];
                }};
            Constraint ThermometerConstraint2{
                [i1, j1, this](int Num)
                {
                    return !Board[K(i1, j1)] || Board[K(i1, j1)] < Num;
                }};
            ExtraConstraints[K(i1, j1)].emplace_back(std::move(ThermometerConstraint1));
            ExtraConstraints[K(i2, j2)].emplace_back(std::move(ThermometerConstraint2));
        }
    }
}

std::istream &operator>>(std::istream &in, ThermometerSudoku &sudoku)
{
    in >> static_cast<Sudoku &>(sudoku);

    /**
     * 温度计额外约束输入格式：
     *   - 每一行一条温度计链，位置按从小到大排列
     *   - x1 y1 x2 y2 x3 y3 ...
     *   - 满足 (x1, y1) < (x2, y2) < (x3, y3) < ...
     */
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::istringstream iss(line);
        std::vector<Sudoku::Position> Thermometer;
        while (iss)
        {
            Sudoku::Position p;
            iss >> p.first >> p.second;
            if (p.first == 0 && p.second == 0)
            {
                break;
            }
            if (p.first != 0 && p.second != 0)
            {
                p.first--;
                p.second--;
                Thermometer.emplace_back(std::move(p));
                continue;
            }
            std::cerr << "- 温度计约束错误：(" << p.first << ", " << p.second << ")" << std::endl;
            exit(1);
        }
        sudoku.Thermometers.emplace_back(std::move(Thermometer));
    }
    return in;
}
