#include "stdafx.h"

#include "common.h"

#include "standard_sudoku.h"
#include "killer_sudoku.h"
#include "thermometer_sudoku.h"
#include "odd_even_sudoku.h"
#include "continuous_sudoku.h"
#include "no_horse_sudoku.h"

namespace
{
	std::string GetCurrentTimestamp()
	{
		const std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();
		const std::time_t timestamp = std::chrono::system_clock::to_time_t(timePoint);
		return std::to_string(timestamp);
	}

	const std::filesystem::path LOG_PATH{"log/"s + GetCurrentTimestamp() + ".log"s};
}

std::shared_ptr<Sudoku> GetSudoku(std::istream& in)
{
	int type = 0;
	in >> type;
	switch (static_cast<SudokuType>(type))
	{
	case SudokuType::None:
		{
			std::ostringstream os;
			os << "Not Support Sudoku Type!\n"sv;
			std::cerr << os.str();
			throw std::runtime_error(os.str());
		}
	case SudokuType::Standard:
		{
			auto sudoku = std::make_shared<StandardSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	case SudokuType::Killer:
		{
			auto sudoku = std::make_shared<KillerSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	case SudokuType::Thermometer:
		{
			auto sudoku = std::make_shared<ThermometerSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	case SudokuType::OddEven:
		{
			auto sudoku = std::make_shared<OddEvenSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	case SudokuType::Continuous:
		{
			auto sudoku = std::make_shared<ContinuousSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	case SudokuType::NoHorse:
		{
			auto sudoku = std::make_shared<NoHorseSudoku>();
			in >> *sudoku;
			return sudoku;
		}
	}
	return nullptr;
}

void SolveSudoku(const std::filesystem::path& path)
{
	std::ostringstream os;
	if (!std::filesystem::exists(path))
	{
		os.str(""s);
		os << "Path Not Found: "sv << path << '\n';
		std::cerr << os.str();
		throw std::runtime_error(os.str());
	}
	std::ifstream is(path);
	std::shared_ptr<Sudoku> sudoku = GetSudoku(is);
	os.str(""s);
	os << "文件: "sv << path << '\n';
#ifdef _DEBUG
	os << "模式: Debug"sv << std::endl;
#elif NDEBUG
	os << "模式: Release"sv << '\n';
#endif
	std::cout << os.str();
	auto start = std::chrono::system_clock::now();
	sudoku->Solve();
	auto end = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	os.str(""s);
	os << "用时: "sv << duration << "\n\n";
	std::cout << os.str();
	std::cout << *sudoku << '\n';
	std::ofstream log(LOG_PATH, std::ios::app);
	log << "文件: "sv << path << '\n';
#ifdef _DEBUG
	log << "模式: Debug\n"sv;
#elif NDEBUG
	log << "模式: Release\n"sv;
#endif
	log << "用时: "sv << duration << "\n\n";
	log << *sudoku << '\n';
}
