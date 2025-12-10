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

static std::shared_ptr<Sudoku> GetSudoku(std::istream &in)
{
	int type = 0;
	in >> type;
	switch (SudokuType(type))
	{
	case SudokuType::Standard:
	{
		std::shared_ptr<Sudoku> sudoku = std::make_shared<Sudoku>();
		in >> *sudoku;
		return sudoku;
	}
	break;
	case SudokuType::NoHorse:
	{
		std::shared_ptr<Sudoku> sudoku = std::make_shared<NoHorseSudoku>();
		in >> *sudoku;
		return sudoku;
	}
	break;
	default:
		std::cerr << "No Support Sudoku Type!" << std::endl;
		exit(1);
	}
}

int main()
{
	std::filesystem::path path(R"(thermometer\sokudo3.txt)");
	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path Not Found: " << path << std::endl;
		return 1;
	}
	std::ifstream is(path);
	std::shared_ptr sudoku = GetSudoku(is);
	std::cout << "*** 开始求解 ***" << std::endl
			  << std::endl;
	auto start = std::chrono::system_clock::now();
	sudoku->Solve();
	auto end = std::chrono::system_clock::now();
	std::cout << *sudoku << std::endl;
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "*** 用时：" << duration << " ***" << std::endl;
	return 0;
}