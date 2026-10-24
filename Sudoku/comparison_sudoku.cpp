#include "stdafx.h"

#include "comparison_sudoku.h"

#include "io.h"

namespace
{
	constexpr auto NAME{"数比数独"sv};

	template <bool IsRowMatrix, size_t M, size_t N>
	void TranslateComparisons(
		const std::array<std::array<char, N>, M>& matrix,
		std::vector<std::pair<Position, Position>>& outComparisons)
	{
		for (size_t i{}; i < M; i++)
		{
			for (size_t j{}; j < N; j++)
			{
				const Position p1{
					IsRowMatrix ? i : j,
					IsRowMatrix ? j : i
				};
				const Position p2{
					IsRowMatrix ? i : j + 1,
					IsRowMatrix ? j + 1 : i
				};
				switch (const char c{matrix[i][j]})
				{
				case '<':
					{
						outComparisons.emplace_back(p1, p2);
						break;
					}
				case '>':
					{
						outComparisons.emplace_back(p2, p1);
						break;
					}
				case '.':
					{
						break;
					}
				default:
					{
						std::ostringstream os;
						os << "- "sv << NAME << "约束错误：("sv << c << ") 关系未知\n"sv;
						std::cerr << os.str();
						throw std::runtime_error(os.str());
					}
				}
			}
		}
	}
}

std::string_view ComparisonSudoku::GetName() const
{
	return NAME;
}

void ComparisonSudoku::InitializeConstraints()
{
	for (auto&& [p1, p2] : Comparisons)
	{
		auto [i1, j1]{p1};
		auto [i2, j2]{p2};
		AddConstraint(i1, j1, [i2, j2](const Sudoku& sudoku, const NumType num)
		{
			return sudoku(i2, j2) == 0 || num < sudoku(i2, j2);
		});
		AddConstraint(i2, j2, [i1, j1](const Sudoku& sudoku, const NumType num)
		{
			return sudoku(i1, j1) == 0 || sudoku(i1, j1) < num;
		});
	}
}

void ComparisonSudoku::Read(std::istream& in)
{
	Super::Read(in);

	/**
	 * 数比数独额外约束输入格式：
	 *   - 两个矩阵
	 *   - 第一个矩阵每行表示每行数字之间的比较关系
	 *   - 第二个矩阵每行表示每列数字之间的比较关系
	 *   - 关系有三种：小于（<）、大于（>）和无关系（.）
	 *   - 矩阵必定是 9 行 8 列
	 */

	constexpr size_t m{9};
	constexpr size_t n{8};

	const auto rowComparisonMatrix = IO::GetMatrix<char, m, n>(in);
	const auto colComparisonMatrix = IO::GetMatrix<char, m, n>(in);

	TranslateComparisons<true>(rowComparisonMatrix, Comparisons);
	TranslateComparisons<false>(colComparisonMatrix, Comparisons);
}
