#include "sudoku_solver.h"

#include "config.h"

SudokuSolver::SudokuSolver(void)
{
	sAppName = "SUDOKU SOLVER";
}

bool SudokuSolver::OnUserUpdate(float fElapsedTime)
{
	Input();
	Logic();
	Draw();
	return true;
}

void SudokuSolver::Input(void)
{

}

void SudokuSolver::Logic(void)
{

}

void SudokuSolver::Draw(void)
{
	Clear(olc::WHITE);
	
	DrawBoundaries();
	DrawStrings();
}

void SudokuSolver::DrawBoundaries(void)
{	
	DrawLine(0, TopLayerHeight, ScreenWidth(), TopLayerHeight, olc::BLACK);
	for (uint8_t index = 0; index < SudokuUnits; index++)
	{
		if (index % 3 == 0)
		{
			DrawLine(0, TopLayerHeight + index * SudokuUnitHeight, BoardWidth, TopLayerHeight + index * SudokuUnitHeight, olc::BLACK);
		}
		else
		{
			DrawLine(0, TopLayerHeight + index * SudokuUnitHeight, BoardWidth, TopLayerHeight + index * SudokuUnitHeight, olc::GREY);
		}

		if (index % 3 == 0)
		{
			DrawLine(BoardWidth - index * SudokuUnitWidth, TopLayerHeight, BoardWidth - index * SudokuUnitWidth, ScreenHeight(), olc::BLACK);
		}
		else
		{
			DrawLine(BoardWidth - index * SudokuUnitWidth, TopLayerHeight, BoardWidth - index * SudokuUnitWidth, ScreenHeight(), olc::GREY);
		}
	}
}

void SudokuSolver::DrawStrings(void)
{
	std::string topLayerString = "Sudoku Solver";
	constexpr uint32_t TextScale = 2;
	int32_t posX = TopLayerWidth / 2 - GetTextSize(topLayerString).x / 2 * TextScale;
	int32_t posY = TopLayerHeight / 2 - GetTextSize(topLayerString).y / 2 * TextScale;

	DrawString(posX, posY, topLayerString, olc::BLACK, TextScale);
}