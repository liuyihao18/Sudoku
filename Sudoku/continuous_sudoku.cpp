#include "stdafx.h"

#include "continuous_sudoku.h"

#include "io.h"

namespace
{
	constexpr auto NAME{"连续数独"sv};

	void Check(const size_t i1, const size_t j1, const size_t i2, const size_t j2)
	{
		if (const int si1{static_cast<int>(i1)}, sj1{static_cast<int>(j1)},
		              si2{static_cast<int>(i2)}, sj2{static_cast<int>(j2)};
			!((abs(si2 - si1) == 0 && abs(sj2 - sj1) == 1) || (abs(si2 - si1) == 1 && abs(sj2 - sj1) == 0)))
		{
			std::ostringstream os;
			os << "- "sv << NAME << "约束错误：("sv
				<< si1 + 1 << ", "sv << sj1 + 1 << ") <-> ("sv << si2 + 1 << ", "sv << sj2 + 1 << ") 不连续\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	}
}

std::string_view ContinuousSudoku::GetName() const
{
	return NAME;
}

void ContinuousSudoku::InitializeConstraints()
{
	for (auto&& [p1, p2] : ContinuousPositions)
	{
		auto [i1, j1]{p1};
		auto [i2, j2]{p2};
		Check(i1, j1, i2, j2);
		AddConstraint(i1, j1, [i2, j2](const Sudoku& sudoku, const NumType num)
		{
			return !sudoku(i2, j2) || sudoku(i2, j2) - num == 1 || num - sudoku(i2, j2) == 1;
		});
		AddConstraint(i2, j2, [i1, j1](const Sudoku& sudoku, const NumType num)
		{
			return !sudoku(i1, j1) || sudoku(i1, j1) - num == 1 || num - sudoku(i1, j1) == 1;
		});
	}
}

void ContinuousSudoku::Read(std::istream& in)
{
	Super::Read(in);

	/**
	 * 连续数独额外约束输入格式：
	 *   - 每一行一条连续约束
	 *   - x1 y1 x2 y2
	 *   - 满足 |Board(x1, y1) - Board(x2, y2)| = 1
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
				os << "- "sv << NAME << "约束错误："sv << positions.size() << " != 2\n"sv;
				std::cerr << os.str();
				throw std::runtime_error(os.str());
			}
			ContinuousPositions.emplace_back(positions[0], positions[1]);
		}
	}
}
