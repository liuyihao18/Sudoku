#include "sudoku.h"
#include "no_horse_sudoku.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>

enum class SudokuType
{
	Standard = 1,
	NoHorse = 2
};

static Sudoku GetSudoku(std::istream &in)
{
	int type = 0;
	in >> type;
	switch (SudokuType(type))
	{
	case SudokuType::Standard:
	{
		Sudoku sudoku;
		in >> sudoku;
		return sudoku;
	}
	break;
	case SudokuType::NoHorse:
	{
		NoHorseSudoku sudoku;
		in >> sudoku;
		return sudoku;
	}
	break;
	default:
		throw std::runtime_error("No Support Sudoku Type!");
	}
}

int main()
{
	std::filesystem::path path(R"(sokudo\sokudo2.txt)");
	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path Not Found: " << path << std::endl;
		return -1;
	}
	std::ifstream is(path);
	Sudoku sudoku = GetSudoku(is);
	std::cout << "*** 开始求解 ***" << std::endl
			  << std::endl;
	auto start = std::chrono::system_clock::now();
	sudoku.Solve();
	auto end = std::chrono::system_clock::now();
	std::cout << sudoku << std::endl;
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "*** 用时：" << duration << " ***" << std::endl;
	return 0;
}