#include "stdafx.h"

#include "odd_even_sudoku.h"

#include "solver.h"

std::string_view OddEvenSudoku::GetName() const
{
	return "奇偶数独"sv;
}

void OddEvenSudoku::InitializeSolver(Solver& solver)
{
	for (auto&& [i, j] : Odd)
	{
		Constraint oddConstraint{
			[this](const NumType num, const Sudoku&)
			{
				return num & 1;
			}
		};
		solver.AddConstraint(i, j, std::move(oddConstraint));
	}
	for (auto&& [i, j] : Even)
	{
		Constraint evenConstraint{
			[this](const NumType num, const Sudoku&)
			{
				return !(num & 1);
			}
		};
		solver.AddConstraint(i, j, std::move(evenConstraint));
	}
}

std::istream& operator>>(std::istream& in, OddEvenSudoku& sudoku)
{
	in >> static_cast<Sudoku&>(sudoku);

	/**
	 * 奇偶数独额外约束输入格式：
	 *   - 第一行为奇数位置：x1 y1 x2 y2 ...
	 *   - 第二行为偶数位置：x1 y1 x2 y2 ...
	 */
	auto input{
		[&in](std::vector<Position>& positions)
		{
			std::string line{};
			while (std::getline(in, line) && line.empty())
			{
			}
			std::istringstream iss(line);
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
					positions.emplace_back(p);
					continue;
				}
				std::ostringstream os;
				os << "- 奇偶数独约束错误：("sv << p.first << ", "sv << p.second << ") 孤立\n"sv;
				std::cerr << os.str();
				throw std::runtime_error(os.str());
			}
		}
	};
	input(sudoku.Odd);
	input(sudoku.Even);
	return in;
}
