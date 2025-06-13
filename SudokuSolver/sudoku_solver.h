#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "olcPixelGameEngine.h"

class SudokuSolver : public olc::PixelGameEngine
{
public:
	SudokuSolver(void);

	bool OnUserCreate(void) override { return true; };
	bool OnUserUpdate(float fElapsedTime) override;

private:
	void Input(void);
	void Logic(void);
	void Draw(void);
};

#endif