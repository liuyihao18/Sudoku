#include "stdafx.h"

#include "odd_even_sudoku.h"

#include "io.h"

namespace
{
	constexpr auto NAME{"奇偶数独"sv};
}

std::string_view OddEvenSudoku::GetName() const
{
	return NAME;
}

void OddEvenSudoku::InitializeConstraints()
{
	for (auto&& [i, j] : Odd)
	{
		AddConstraint(i, j, [](const Sudoku&, const NumType num)
		{
			return num & 1;
		});
	}
	for (auto&& [i, j] : Even)
	{
		AddConstraint(i, j, [](const Sudoku&, const NumType num)
		{
			return !(num & 1);
		});
	}
}

void OddEvenSudoku::Read(std::istream& in)
{
	Super::Read(in);

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
			IO::ParseLine(line, positions, NAME);
		}
	};

	input(Odd);
	input(Even);
}
