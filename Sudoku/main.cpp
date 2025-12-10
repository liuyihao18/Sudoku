#include "sudoku.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>

int main()
{
	std::filesystem::path path(R"(sokudo\sokudo2.txt)");
	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path Not Found: " << path << std::endl;
		return -1;
	}
	std::ifstream is(path);
	Sudoku sudoku;
	is >> sudoku;
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