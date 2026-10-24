#pragma once
#include "sudoku.h"

class ThermometerSudoku final : public Sudoku
{
	using Super = Sudoku;

	using Thermometer = std::vector<Position>;

public:
	[[nodiscard]] std::string_view GetName() const override;
	void InitializeConstraints() override;

	void Read(std::istream& in) override;

private:
	std::vector<Thermometer> Thermometers;
};
