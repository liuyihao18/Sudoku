#include "stdafx.h"

#include "common.h"
#include "test.h"

int main(const int argc, char** argv)
{
	try
	{
		if (argc > 1)
		{
			SolveSudoku(argv[1]);
		}
		else
		{
			TestAll();
		}
	}
	catch (const std::runtime_error&)
	{
		return -1;
	}
	return 0;
}
