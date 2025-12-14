#include "stdafx.h"

#include "continuous_sudoku.h"

#include "solver.h"

namespace
{
	void Check(const size_t i1, const size_t j1, const size_t i2, const size_t j2)
	{
		if (const int si1{static_cast<int>(i1)}, sj1{static_cast<int>(j1)},
		              si2{static_cast<int>(i2)}, sj2{static_cast<int>(j2)};
			!((abs(si2 - si1) == 0 && abs(sj2 - sj1) == 1) || (abs(si2 - si1) == 1 && abs(sj2 - sj1) == 0)))
		{
			std::ostringstream os;
			os << "- 连续数独约束错误：("sv << si1 + 1 << ", "sv << sj1 + 1 << ") <-> ("sv
				<< si2 + 1 << ", "sv << sj2 + 1 << ") 不连续\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	}
}

std::string_view ContinuousSudoku::GetName() const
{
	return "连续数独"sv;
}

void ContinuousSudoku::InitializeSolver(Solver& solver)
{
	for (const std::vector<Position>& continuousPosition : ContinuousPositions)
	{
		auto&& [i1, j1]{continuousPosition[0]};
		auto&& [i2, j2]{continuousPosition[1]};
		Check(i1, j1, i2, j2);
		Constraint continuousConstraint1{
			[i2, j2, this](const NumType num, const Sudoku& sudoku)
			{
				return !sudoku(i2, j2) || sudoku(i2, j2) - num == 1 || num - sudoku(i2, j2) == 1;
			}
		};
		Constraint continuousConstraint2{
			[i1, j1, this](const NumType num, const Sudoku& sudoku)
			{
				return !sudoku(i1, j1) || sudoku(i1, j1) - num == 1 || num - sudoku(i1, j1) == 1;
			}
		};
		solver.AddConstraint(i1, j1, std::move(continuousConstraint1));
		solver.AddConstraint(i2, j2, std::move(continuousConstraint2));
	}
}

std::istream& operator>>(std::istream& in, ContinuousSudoku& sudoku)
{
	in >> static_cast<Sudoku&>(sudoku);

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
		std::vector<Position> continuousPosition;
		while (iss)
		{
			Position p{};
			iss >> p.first >> p.second;
			if (p.first == 0 && p.second == 0)
			{
				break;
			}
			if (p.first != 0 && p.second != 0)
			{
				p.first--;
				p.second--;
				continuousPosition.emplace_back(p);
				continue;
			}
			std::ostringstream os;
			os << "- 连续数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
		if (continuousPosition.size() > 2)
		{
			std::ostringstream os;
			os << "- 连续数独约束错误："sv << continuousPosition.size() << " > 2\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
		sudoku.ContinuousPositions.emplace_back(std::move(continuousPosition));
	}
	return in;
}
