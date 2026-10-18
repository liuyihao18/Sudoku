#include "stdafx.h"

#include "arrow_sudoku.h"

#include "solver.h"

namespace
{
	void Check(const size_t i1, const size_t j1, const size_t i2, const size_t j2)
	{
		if (const int si1{static_cast<int>(i1)}, sj1{static_cast<int>(j1)},
		              si2{static_cast<int>(i2)}, sj2{static_cast<int>(j2)};
			abs(si2 - si1) > 1 || abs(sj2 - sj1) > 1)
		{
			std::ostringstream os;
			os << "- 箭头数独约束错误：("sv << si1 + 1 << ", "sv << sj1 + 1 << ") < ("sv << si2 + 1 << ", "sv << sj2 + 1 <<
				") 不连续\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	}
}

std::string_view ArrowSudoku::GetName() const
{
	return "箭头数独"sv;
}

void ArrowSudoku::InitializeSolver(Solver& solver)
{
	for (const Arrow& arrow : Arrows)
	{
		for (size_t index{0}, n{arrow.size()}; index + 1 < n; index++)
		{
			const auto& [i1, j1]{arrow[index]};
			const auto& [i2, j2]{arrow[index + 1]};
			Check(i1, j1, i2, j2);
		}
		for (auto&& [i, j] : arrow)
		{
			ConstraintType arrowConstraint{
				[arrow, i, j](const NumType num, const Sudoku& sudoku)
				{
					bool isStart;
					NumType sum;
					if (const auto& [startI, startJ]{arrow[0]};
						i == startI && j == startJ)
					{
						isStart = true;
						sum = num;
					}
					else
					{
						isStart = false;
						sum = sudoku(startI, startJ);
					}

					bool hasZero{false};
					NumType temp{0};
					for (size_t index{1}, n{arrow.size()}; index < n; index++)
					{
						const auto& [ii, jj] = arrow[index];
						if (ii == i && jj == j)
						{
							continue;
						}
						hasZero |= sudoku(ii, jj) == 0;
						temp += sudoku(ii, jj);
					}

					return isStart
						       ? (hasZero ? temp < sum : temp == sum)
						       : sum == 0 || (hasZero ? temp + num < sum : temp + num == sum);
				}
			};
			solver.AddConstraint(i, j, std::move(arrowConstraint));
		}
	}
}

void ArrowSudoku::Read(std::istream& in)
{
	Super::Read(in);

	/**
	 * 箭头数独额外约束输入格式：
	 *   - 每一行一个箭头，第一个元素是箭头起点
	 *   - x1 y1 x2 y2 x3 y3 ...
	 *   - 满足 Board(x1, y1) = Board(x2, y2) + Board(x3, y3) + ...
	 */
	std::string line{};
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			continue;
		}
		std::istringstream iss(line);
		Arrow arrow;
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
				arrow.emplace_back(p);
				continue;
			}
			std::ostringstream os;
			os << "- 箭头数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
		Arrows.emplace_back(std::move(arrow));
	}
}
