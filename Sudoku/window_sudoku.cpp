#include "stdafx.h"

#include "window_sudoku.h"

#include "io.h"

namespace
{
	constexpr auto NAME{"窗口数独"sv};

	void Check(const size_t i1, const size_t j1, const size_t i2, const size_t j2)
	{
		if (i1 >= i2 || j1 >= j2
			|| (i2 - i1) * (j2 - j1) > 9)
		{
			std::ostringstream os;
			os << "- "sv << NAME << "约束错误：("sv
				<< i1 + 1 << ", "sv << j1 + 1 << ") - ("sv << i2 + 1 << ", "sv << j2 + 1 << ") 异常窗口\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	}
}

std::string_view WindowSudoku::GetName() const
{
	return NAME;
}

void WindowSudoku::InitializeConstraints()
{
	for (auto&& [p1, p2] : Windows)
	{
		auto [i1, j1]{p1};
		auto [i2, j2]{p2};
		Check(i1, j1, i2, j2);

		for (size_t i{i1}; i <= i2; i++)
		{
			for (size_t j{j1}; j <= j2; j++)
			{
				AddConstraint(i, j, [i1, j1,i2,j2](const Sudoku& sudoku, const NumType num)
				{
					for (size_t ii{i1}; ii <= i2; ii++)
					{
						for (size_t jj{j1}; jj <= j2; jj++)
						{
							if (num == sudoku(ii, jj))
							{
								return false;
							}
						}
					}
					return true;
				});
			}
		}
	}
}

void WindowSudoku::Read(std::istream& in)
{
	Sudoku::Read(in);

	/**
	 * 窗口数独额外约束输入格式：
	 *   - 每一行为一个窗口：x1 y1 x2 y2
	 */
	std::string line{};
	while (std::getline(in, line))
	{
		if (std::vector<Position> positions;
			IO::ParseLine(line, positions, NAME))
		{
			if (positions.size() != 2)
			{
				std::ostringstream os;
				os << "- "sv << NAME << "约束错误："sv << line << " 点数错误\n"sv;
				std::cerr << os.str();
				throw std::runtime_error(os.str());
			}
			Windows.emplace_back(positions[0], positions[1]);
		}
	}
}
