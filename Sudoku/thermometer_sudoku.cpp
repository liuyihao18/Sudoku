#include "stdafx.h"

#include "thermometer_sudoku.h"

namespace
{
	void Check(const size_t i1, const size_t j1, const size_t i2, const size_t j2)
	{
		if (const int si1{static_cast<int>(i1)}, sj1{static_cast<int>(j1)},
		              si2{static_cast<int>(i2)}, sj2{static_cast<int>(j2)};
			abs(si2 - si1) > 1 || abs(sj2 - sj1) > 1)
		{
			std::ostringstream os;
			os << "- 温度计数独约束错误：("sv << si1 + 1 << ", "sv << sj1 + 1 << ") < ("sv << si2 + 1 << ", "sv << sj2 + 1 <<
				") 不连续\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	}
}

std::string_view ThermometerSudoku::GetName() const
{
	return "温度计数独"sv;
}

void ThermometerSudoku::InitializeExtraConstraints()
{
	for (const std::vector<Position>& Thermometer : Thermometers)
	{
		const size_t n{Thermometer.size()};
		for (size_t m{}; m + 1 < n; m++)
		{
			auto&& [i1, j1]{Thermometer[m]};
			auto&& [i2, j2]{Thermometer[m + 1]};
			Check(i1, j1, i2, j2);
			Constraint thermometerConstraint1{
				[i2, j2, this](const NumType num, const BoardType& board)
				{
					return !board[K(i2, j2)] || num < board[K(i2, j2)];
				}
			};
			Constraint thermometerConstraint2{
				[i1, j1, this](const NumType num, const BoardType& board)
				{
					return !board[K(i1, j1)] || board[K(i1, j1)] < num;
				}
			};
			(*ExtraConstraints)[K(i1, j1)].emplace_back(std::move(thermometerConstraint1));
			(*ExtraConstraints)[K(i2, j2)].emplace_back(std::move(thermometerConstraint2));
		}
	}
}

std::istream& operator>>(std::istream& in, ThermometerSudoku& sudoku)
{
	in >> static_cast<Sudoku&>(sudoku);

	/**
	 * 温度计数独额外约束输入格式：
	 *   - 每一行一条温度计链，位置按从小到大排列
	 *   - x1 y1 x2 y2 x3 y3 ...
	 *   - 满足 (x1, y1) < (x2, y2) < (x3, y3) < ...
	 */
	std::string line{};
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			continue;
		}
		std::istringstream iss(line);
		std::vector<Sudoku::Position> thermometer;
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
				thermometer.emplace_back(p);
				continue;
			}
			std::ostringstream os;
			os << "- 温度计数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
		sudoku.Thermometers.emplace_back(std::move(thermometer));
	}
	return in;
}
