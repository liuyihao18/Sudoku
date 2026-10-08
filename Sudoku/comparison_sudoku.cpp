#include "stdafx.h"

#include "comparison_sudoku.h"

#include "solver.h"

namespace
{
	template <typename T, size_t M, size_t N>
	std::array<std::array<T, N>, M> GetMatrix(std::istream& in)
	{
		std::array<std::array<T, N>, M> matrix{};
		for (size_t i{}; i < M; i++)
		{
			for (size_t j{}; j < N; j++)
			{
				in >> matrix[i][j];
			}
		}
		return matrix;
	}

	template <bool isTransposed, size_t M, size_t N>
	void TranslateComparisons(
		const std::array<std::array<char, N>, M>& matrix,
		std::vector<std::pair<Position, Position>>& outComparisons)
	{
		for (size_t i{}; i < M; i++)
		{
			for (size_t j{}; j < N; j++)
			{
				const Position p1{
					isTransposed ? i : j,
					isTransposed ? j : i
				};
				const Position p2{
					isTransposed ? i : j + 1,
					isTransposed ? j + 1 : i
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
						os << "- 数比数独约束错误：("sv << c << ") 关系未知\n"sv;
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
	return "数比数独"sv;
}

void ComparisonSudoku::InitializeSolver(Solver& solver)
{
	for (auto&& [p1, p2] : Comparisons)
	{
		ConstraintType ComparisonConstraint1{
			[p2](NumType num, const Sudoku& sudoku)
			{
				return sudoku(p2.first, p2.second) == 0
					|| num < sudoku(p2.first, p2.second);
			}
		};
		ConstraintType ComparisonConstraint2{
			[p1](NumType num, const Sudoku& sudoku)
			{
				return sudoku(p1.first, p1.second) == 0
					|| sudoku(p1.first, p1.second) < num;
			}
		};
		solver.AddConstraint(p1.first, p1.second, ComparisonConstraint1);
		solver.AddConstraint(p2.first, p2.second, ComparisonConstraint2);
	}
}

std::istream& operator>>(std::istream& in, ComparisonSudoku& sudoku)
{
	in >> static_cast<Sudoku&>(sudoku);

	/**
	 * 数比数独额外约束输入格式：
	 *   - 两个矩阵
	 *   - 第一个矩阵每行表示每行数字之间的比较关系
	 *   - 第二个矩阵每行表示每列数字之间的比较关系
	 *   - 关系有三种：小于（<）、大于（>）和无关系（.）
	 *   - 矩阵必定是 9 行 8 列
	 */

	constexpr size_t M{9};
	constexpr size_t N{8};

	const auto rowComparisonMatrix = GetMatrix<char, M, N>(in);
	const auto colComparisonMatrix = GetMatrix<char, M, N>(in);

	TranslateComparisons<true>(rowComparisonMatrix, sudoku.Comparisons);
	TranslateComparisons<false>(colComparisonMatrix, sudoku.Comparisons);

	return in;
}
