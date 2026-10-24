#pragma once
#include "constant.h"

namespace IO
{
	bool ParseLine(const std::string& line, std::vector<Position>& positions,
	               std::string_view sudokuName = ""sv);

	bool ParseLine(const std::string& line, NumType& headNum, std::vector<Position>& positions,
	               std::string_view sudokuName = ""sv);

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
}
