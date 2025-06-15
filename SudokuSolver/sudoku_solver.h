#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stack>

#include "config.h"
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
	void DrawBoundaries(void);
	void DrawStrings(void);
	void DrawBoard(void);
	void DrawHelpText(void);

	struct Step
	{
		uint8_t col;
		uint8_t row;
		uint8_t candidate;
	};
	bool Solve(void);
	void InitStack(void);

	void ResetBoard(void);

	bool IsValid(uint8_t column, uint8_t row, uint8_t number);

	enum class State
	{
		Idle,
		WaitingForInput,
		Running,
		Pause,
		Reset,
		Finished
	};
	State _state = State::Idle;

	struct Cell
	{
		uint8_t value = 0;
		bool isFixed = false;
	};
	Cell _board[SudokuUnits][SudokuUnits] = {};

	int32_t _mouseX;
	int32_t _mouseY;

	uint8_t _activeCellX;
	uint8_t _activeCellY;

	bool _inputValid = true;

	std::stack<Step> _stack;
};

#endif