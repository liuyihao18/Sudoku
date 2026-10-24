#include "stdafx.h"

#include "killer_sudoku.h"

#include "io.h"

namespace
{
	constexpr auto NAME{"杀手数独"sv};

	void Check(const std::vector<Position>& positions)
	{
		auto distance{
			[](const Position& p1, const Position& p2)
			{
				const size_t deltaI{p1.first <= p2.first ? p2.first - p1.first : p1.first - p2.first};
				const size_t deltaJ{p1.second <= p2.second ? p2.second - p1.second : p1.second - p2.second};
				return deltaI + deltaJ;
			}
		};

		for (size_t i{}, n{positions.size()}; i < n; i++)
		{
			size_t minDistance{0xffffffff};
			Position p1{positions[i]};
			for (size_t j{}; j < n; j++)
			{
				if (j == i)
				{
					continue;
				}
				Position p2{positions[j]};
				minDistance = std::min(minDistance, distance(p1, p2));
			}

			if (minDistance != 1)
			{
				std::ostringstream os;
				os << "- "sv << NAME << "约束错误：("sv
					<< p1.first + 1 << ", "sv << p1.second + 1 << ") 不连续\n"sv;
				std::cerr << os.str();
				throw std::runtime_error(os.str());
			}
		}
	}
}

std::string_view KillerSudoku::GetName() const
{
	return NAME;
}

void KillerSudoku::InitializeConstraints()
{
	for (const Killer& killer : Killers)
	{
		Check(killer.Positions);
		for (auto&& [i, j] : killer.Positions)
		{
			AddConstraint(i, j, [killer, i, j](const Sudoku& sudoku, const NumType num)
			{
				bool hasZero{false};
				NumType temp{0};
				for (auto&& [ii, jj] : killer.Positions)
				{
					if (ii == i && jj == j)
					{
						continue;
					}
					hasZero |= sudoku(ii, jj) == 0;
					temp += sudoku(ii, jj);
				}
				return hasZero ? temp + num < killer.Sum : temp + num == killer.Sum;
			});
		}
	}
}

void KillerSudoku::Read(std::istream& in)
{
	Super::Read(in);

	/**
	 * 杀手数独额外约束输入格式：
	 *   - 每一行一个范围和
	 *   - sum x1 y1 x2 y2 ...
	 *   - 满足 sum = Board(x1, y1) + Board(x2, y2) + ...
	 */
	std::string line{};
	while (std::getline(in, line))
	{
		if (Killer killer;
			IO::ParseLine(line, killer.Sum, killer.Positions, NAME))
		{
			Killers.emplace_back(std::move(killer));
		}
	}
}
