#include "stdafx.h"

#include "io.h"

bool IO::ParseLine(const std::string& line, std::vector<Position>& positions, const std::string_view sudokuName)
{
	if (line.empty())
	{
		return false;
	}

	std::istringstream iss(line);
	while (iss)
	{
		Position p{};
		iss >> p.first >> p.second;
		if (p.first == 0 && p.second == 0)
		{
			break;
		}
		if (p.first != 0 && p.second != 0)
		{
			p.first--;
			p.second--;
			positions.emplace_back(p);
			continue;
		}
		std::ostringstream os;
		os << "- "sv << sudokuName << "约束错误：("sv
			<< p.first << ", "sv << p.second << ") 不成组\n"sv;
		std::cerr << os.str();
		throw std::runtime_error(os.str());
	}

	return true;
}

bool IO::ParseLine(const std::string& line, NumType& headNum, std::vector<Position>& positions,
                   std::string_view sudokuName)
{
	if (line.empty())
	{
		return false;
	}

	std::istringstream iss(line);
	iss >> headNum;
	while (iss)
	{
		Position p{};
		iss >> p.first >> p.second;
		if (p.first == 0 && p.second == 0)
		{
			break;
		}
		if (p.first != 0 && p.second != 0)
		{
			p.first--;
			p.second--;
			positions.emplace_back(p);
			continue;
		}
		std::ostringstream os;
		os << "- "sv << sudokuName << "约束错误：("sv
			<< p.first << ", "sv << p.second << ") 不成组\n"sv;
		std::cerr << os.str();
		throw std::runtime_error(os.str());
	}

	return true;
}
