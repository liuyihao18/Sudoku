#include "stdafx.h"

#include "common.h"

#include "standard_sudoku.h"
#include "killer_sudoku.h"
#include "thermometer_sudoku.h"
#include "odd_even_sudoku.h"
#include "continuous_sudoku.h"
#include "no_horse_sudoku.h"

std::shared_ptr<Sudoku> GetSudoku(std::istream &in)
{
    int type = 0;
    in >> type;
    switch (SudokuType(type))
    {
    case SudokuType::Standard:
    {
        std::shared_ptr<StandardSudoku> sudoku = std::make_shared<StandardSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    case SudokuType::Killer:
    {
        std::shared_ptr<KillerSudoku> sudoku = std::make_shared<KillerSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    case SudokuType::Thermometer:
    {
        std::shared_ptr<ThermometerSudoku> sudoku = std::make_shared<ThermometerSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    case SudokuType::OddEven:
    {
        std::shared_ptr<OddEvenSudoku> sudoku = std::make_shared<OddEvenSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    case SudokuType::Continuous:
    {
        std::shared_ptr<ContinuousSudoku> sudoku = std::make_shared<ContinuousSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    case SudokuType::NoHorse:
    {
        std::shared_ptr<NoHorseSudoku> sudoku = std::make_shared<NoHorseSudoku>();
        in >> *sudoku;
        return sudoku;
    }
    break;
    default:
        std::ostringstream os;
        os << "Not Support Sudoku Type!"sv << std::endl;
        std::cerr << os.str();
        exit(1);
    }
}

static const std::string GetCurrentTimestamp()
{
    std::chrono::system_clock::time_point TimePoint = std::chrono::system_clock::now();
    std::time_t Timestamp = std::chrono::system_clock::to_time_t(TimePoint);
    return std::to_string(Timestamp);
}
static const std::filesystem::path LogPath{"log/"s + GetCurrentTimestamp() + ".log"s};

void SolveSudoku(const std::filesystem::path &Path)
{
    std::ostringstream os;
    if (!std::filesystem::exists(Path))
    {
        os.str(""s);
        os << "Path Not Found: "sv << Path << std::endl;
        std::cerr << os.str();
        exit(1);
    }
    std::ifstream Is(Path);
    std::shared_ptr<Sudoku> Sudoku = GetSudoku(Is);
    os.str(""s);
    os << "文件: "sv << Path << std::endl;
#ifdef _DEBUG
    os << "模式: Debug"sv << std::endl;
#elif NDEBUG
    os << "模式: Release"sv << std::endl;
#endif
    std::cout << os.str();
    auto Start = std::chrono::system_clock::now();
    Sudoku->Solve();
    auto End = std::chrono::system_clock::now();
    auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);
    os.str(""s);
    os << "用时: "sv << Duration << std::endl
       << std::endl;
    std::cout << os.str();
    std::cout << *Sudoku << std::endl;
    std::ofstream Log(LogPath, std::ios::app);
    Log << "文件: "sv << Path << std::endl;
#ifdef _DEBUG
    Log << "模式: Debug"sv << std::endl;
#elif NDEBUG
    Log << "模式: Release"sv << std::endl;
#endif
    Log << "用时: "sv << Duration << std::endl
        << std::endl;
    Log << *Sudoku << std::endl;
}
