#pragma once

enum class SudokuType : std::uint8_t
{
	None = 0,
	Standard = 1,    // 标准数独
	Killer = 2,      // 杀手数独
	Thermometer = 3, // 温度计数独
	OddEven = 4,     // 奇偶数独
	Continuous = 5,  // 连续数独
	NoHorse = 6,     // 无马数独
	Diagonal = 7,    // 对角线数独
	Comparison = 8,  // 数比数独
};

class Sudoku;
std::shared_ptr<Sudoku> GetSudoku(std::istream& in);

void SolveSudoku(const std::filesystem::path& path);
