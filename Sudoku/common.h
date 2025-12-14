#pragma once

enum class SudokuType : std::uint8_t
{
	None = 0,
	Standard = 1,
	Killer = 2,
	Thermometer = 3,
	OddEven = 4,
	Continuous = 5,
	NoHorse = 6,
};

class Sudoku;
std::shared_ptr<Sudoku> GetSudoku(std::istream& in);

void SolveSudoku(const std::filesystem::path& path);
