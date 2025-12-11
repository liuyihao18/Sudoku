#include "thermometer_sudoku.h"
#include <string>

std::istream& operator>>(std::istream& in, ThermometerSudoku& sudoku)
{
	in >> static_cast<Sudoku&>(sudoku);
	std::string line;
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			continue;
		}
		// TODO: 将每一行转换为一个规则
	}
	return in;
}
