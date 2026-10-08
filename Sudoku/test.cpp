#include "stdafx.h"

#include "common.h"

namespace
{
	void TestSudoku(const std::string& dirName)
	{
		for (size_t i = 1; i <= 5; i++)
		{
			const auto filename = "sudoku"s + std::to_string(i) + ".txt"s;
			std::filesystem::path path(dirName + filename);
			SolveSudoku(path);
		}
	}

	void TestStandardSudoku()
	{
		TestSudoku("standard/"s);
	}

	void TestKillerSudoku()
	{
		TestSudoku("killer/"s);
	}

	void TestThermometerSudoku()
	{
		TestSudoku("thermometer/"s);
	}

	void TestOddEvenSudoku()
	{
		TestSudoku("odd_even/"s);
	}

	void TestContinuousSudoku()
	{
		TestSudoku("continuous/"s);
	}

	void TestNoHorseSudoku()
	{
		TestSudoku("no_horse/"s);
	}

	void TestDiagonalSudoku()
	{
		TestSudoku("diagonal/"s);
	}

	void TestComparisonSudoku()
	{
		TestSudoku("comparison/"s);
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
	TestDiagonalSudoku();
	TestComparisonSudoku();

	const auto end = std::chrono::system_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "*** 总用时："sv << duration << " ***\n"sv;
}
