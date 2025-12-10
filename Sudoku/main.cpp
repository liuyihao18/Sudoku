#include "sudoku.h"
#include <fstream>
#include <filesystem>
#include <iostream>

int main()
{
	std::filesystem::path path(R"(board3.txt)");
	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path Not Found: " << path << std::endl;
		return -1;
	}
	std::ifstream is(path);
	Sudoku sudoku;
	is >> sudoku;
	sudoku.solve();
	std::cout << sudoku << std::endl;
	return 0;
}