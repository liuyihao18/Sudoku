#include "stdafx.h"

#include "common.h"

namespace
{
	void TestStandardSudoku()
	{
		const auto dirName = "standard/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestKillerSudoku()
	{
		const auto dirName = "killer/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestThermometerSudoku()
	{
		const auto dirName = "thermometer/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestOddEvenSudoku()
	{
		const auto dirName = "odd_even/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestContinuousSudoku()
	{
		const auto dirName = "continuous/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestNoHorseSudoku()
	{
		const auto dirName = "no_horse/"s;
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}
}

void TestAll()
{
	const auto start = std::chrono::system_clock::now();
	std::cout << "*** 开始批量测试 ***\n\n"sv;

	TestStandardSudoku();
	TestKillerSudoku();
	TestThermometerSudoku();
	TestOddEvenSudoku();
	TestContinuousSudoku();
	TestNoHorseSudoku();

	const auto end = std::chrono::system_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "*** 总用时："sv << duration << " ***\n"sv;
}
