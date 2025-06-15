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