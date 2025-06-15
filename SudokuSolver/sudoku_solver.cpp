#include "sudoku_solver.h"

#include "config.h"

SudokuSolver::SudokuSolver(void)
{
	sAppName = "SUDOKU SOLVER";

	ResetBoard();
}

void SudokuSolver::ResetBoard(void)
{
	for (uint8_t indexX = 0; indexX < SudokuUnits; indexX++)
	{
		for (uint8_t indexY = 0; indexY < SudokuUnits; indexY++)
		{
			_board[indexX][indexY].value = 0;
			_board[indexX][indexY].isFixed = false;
		}
	}
	_mouseX = 0;
	_mouseY = 0;

	_activeCellX = 0;
	_activeCellY = 0;

	_inputValid = true;

	while (!_stack.empty())
	{
		_stack.pop();
	}

	_iterations = 0;
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
	_mouseX = GetMouseX();
	_mouseY = GetMouseY();
	bool mousePressed = false;
	bool numberTyped = false;
	uint8_t number = 255;

	if (GetMouse(0).bPressed || GetMouse(1).bPressed || GetMouse(2).bPressed)
	{
		mousePressed = true;
	}
	if (GetKey(olc::Key::NP0).bHeld || GetKey(olc::Key::K0).bHeld) { numberTyped = true; number = 0; }
	else if (GetKey(olc::Key::NP1).bHeld || GetKey(olc::Key::K1).bHeld) { numberTyped = true; number = 1; }
	else if (GetKey(olc::Key::NP2).bHeld || GetKey(olc::Key::K2).bHeld) { numberTyped = true; number = 2; }
	else if (GetKey(olc::Key::NP3).bHeld || GetKey(olc::Key::K3).bHeld) { numberTyped = true; number = 3; }
	else if (GetKey(olc::Key::NP4).bHeld || GetKey(olc::Key::K4).bHeld) { numberTyped = true; number = 4; }
	else if (GetKey(olc::Key::NP5).bHeld || GetKey(olc::Key::K5).bHeld) { numberTyped = true; number = 5; }
	else if (GetKey(olc::Key::NP6).bHeld || GetKey(olc::Key::K6).bHeld) { numberTyped = true; number = 6; }
	else if (GetKey(olc::Key::NP7).bHeld || GetKey(olc::Key::K7).bHeld) { numberTyped = true; number = 7; }
	else if (GetKey(olc::Key::NP8).bHeld || GetKey(olc::Key::K8).bHeld) { numberTyped = true; number = 8; }
	else if (GetKey(olc::Key::NP9).bHeld || GetKey(olc::Key::K9).bHeld) { numberTyped = true; number = 9; }

	switch (_state)
	{
		case State::Idle:
			if (mousePressed && _mouseX < BoardWidth && _mouseY < ScreenHeight() && _mouseY > TopLayerHeight)
			{
				_activeCellX = _mouseX / SudokuUnitWidth;
				_activeCellY = (_mouseY - TopLayerHeight) / SudokuUnitHeight;
				_state = State::WaitingForInput;
			}
			if (mousePressed && _mouseX > StartStringX && _mouseX < StartStringX + StartStringWidth &&
				_mouseY > StartStringY && _mouseY < StartStringY + StartStringHeight)
			{
				InitStack();
				_state = State::Running;
			}
			if (mousePressed && _mouseX > ResetStringX && _mouseX < ResetStringX + ResetStringWidth &&
				_mouseY > ResetStringY && _mouseY < ResetStringY + ResetStringHeight)
			{
				ResetBoard();
				_state = State::Idle; // Stay in idle
			}
			break;
		case State::WaitingForInput:
			if (numberTyped)
			{
				if (!IsValid(_activeCellX, _activeCellY, number) && number != 0)
				{
					_inputValid = false;
					break;
				}
				_board[_activeCellX][_activeCellY].value = number;
				if (number != 0)
				{
					_board[_activeCellX][_activeCellY].isFixed = true;
				}
				else
				{
					_board[_activeCellX][_activeCellY].isFixed = false;
				}
				_inputValid = true;
				_state = State::Idle;
			}
			break;
		case State::Running:
			if (mousePressed && _mouseX > StartStringX && _mouseX < StartStringX + StartStringWidth &&
				_mouseY > StartStringY && _mouseY < StartStringY + StartStringHeight)
			{
				_state = State::Pause;
			}
			if (mousePressed && _mouseX > ResetStringX && _mouseX < ResetStringX + ResetStringWidth &&
				_mouseY > ResetStringY && _mouseY < ResetStringY + ResetStringHeight)
			{
				ResetBoard();
				_state = State::Idle;
			}
			break;
		case State::Pause:
			if (mousePressed && _mouseX > StartStringX && _mouseX < StartStringX + StartStringWidth &&
				_mouseY > StartStringY && _mouseY < StartStringY + StartStringHeight)
			{
				_state = State::Running;
			}
			break;
		case State::Finished:
			if (mousePressed && _mouseX > ResetStringX && _mouseX < ResetStringX + ResetStringWidth &&
				_mouseY > ResetStringY && _mouseY < ResetStringY + ResetStringHeight)
			{
				ResetBoard();
				_state = State::Idle;
				break;
			}
			break;
		default:
			break;
	}

}

void SudokuSolver::Logic(void)
{
	if (_state == State::Running)
	{
		bool solve = Solve();
		_iterations++;

		bool isValid = true;
		for (uint8_t indexX = 0; indexX < SudokuUnits; indexX++)
		{
			for (uint8_t indexY = 0; indexY < SudokuUnits; indexY++)
			{
				if (!IsValid(indexX, indexY, _board[indexX][indexY].value) || _board[indexX][indexY].value == 0)
				{
					isValid = false;
				}
			}
		}
		if (isValid)
		{
			_state = State::Finished;
		}

	}
}

void SudokuSolver::InitStack(void)
{
	uint8_t col = 0, row = 0;
	while (col < SudokuUnits && _board[col][row].isFixed)
	{
		if (++col >= SudokuUnits)
		{
			col = 0;
			++row;
		}
	}
	_stack.push({ col, row, 0 });
}

bool SudokuSolver::Solve(void)
{
	while (!_stack.empty())
	{
		Step& top = _stack.top();
		uint8_t col = top.col;
		uint8_t row = top.row;

		if (_board[col][row].isFixed)
		{
			_stack.pop();
			continue;
		}

		bool placed = false;
		for (uint8_t n = top.candidate + 1; n <= SudokuUnits; n++)
		{
			if (IsValid(col, row, n))
			{
				_board[col][row].value = n;
				top.candidate = n;

				uint8_t nextCol = col;
				uint8_t nextRow = row;

				do
				{
					if (++nextCol >= SudokuUnits)
					{
						nextCol = 0;
						++nextRow;
					}
				} while (nextRow < SudokuUnits && _board[nextCol][nextRow].isFixed);

				if (nextRow < SudokuUnits)
				{
					_stack.push({ nextCol, nextRow, 0 });
				}

				placed = true;
				break;
			}
		}

		if (!placed)
		{
			_board[col][row].value = 0;
			_stack.pop();
		}

		return true;
	}

	return false;
}

bool SudokuSolver::IsValid(uint8_t x, uint8_t y, uint8_t number)
{
	for (uint8_t col = 0; col < SudokuUnits; col++)
	{
		if (col != x && _board[col][y].value == number)
		{
			return false;
		}
	}

	for (uint8_t row = 0; row < SudokuUnits; row++)
	{
		if (row != y && _board[x][row].value == number)
		{
			return false;
		}
	}

	uint8_t blockStartX = (x / BlockUnits) * BlockUnits;
	uint8_t blockStartY = (y / BlockUnits) * BlockUnits;

	for (uint8_t dx = 0; dx < BlockUnits; ++dx)
	{
		for (uint8_t dy = 0; dy < BlockUnits; ++dy)
		{
			uint8_t cx = blockStartX + dx;
			uint8_t cy = blockStartY + dy;
			if ((cx != x || cy != y) && _board[cx][cy].value == number)
			{
				return false;
			}
		}
	}

	return true;
}

void SudokuSolver::Draw(void)
{
	Clear(olc::WHITE);
	
	DrawBoundaries();
	DrawStrings();
	DrawBoard();
	DrawHelpText();
}

void SudokuSolver::DrawBoundaries(void)
{	
	DrawLine(0, TopLayerHeight, ScreenWidth(), TopLayerHeight, olc::BLACK);
	for (uint8_t index = 0; index < SudokuUnits; index++)
	{
		if (index % BlockUnits == 0)
		{
			DrawLine(0, TopLayerHeight + index * SudokuUnitHeight, BoardWidth, TopLayerHeight + index * SudokuUnitHeight, olc::BLACK);
		}
		else
		{
			DrawLine(0, TopLayerHeight + index * SudokuUnitHeight, BoardWidth, TopLayerHeight + index * SudokuUnitHeight, olc::GREY);
		}

		if (index % BlockUnits == 0)
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

	std::string startString = _state != State::Running ? "Start" : "Pause";
	posX = BoardWidth + SudokuUnitWidth;
	posY = static_cast<int32_t>(TopLayerHeight + SudokuUnitHeight * 1.25);
	DrawString(posX, posY, startString, olc::BLACK, TextScale);
	DrawRect(StartStringX, StartStringY, StartStringWidth, StartStringHeight, olc::BLACK);

	std::string resetString = _state == State::Running ? "Stop" : "Reset";
	posX = BoardWidth + SudokuUnitWidth;
	posY = static_cast<int32_t>(TopLayerHeight + SudokuUnitHeight * 2.25);
	DrawString(posX, posY, resetString, olc::BLACK, TextScale);
	DrawRect(ResetStringX, ResetStringY, ResetStringWidth, ResetStringHeight, olc::BLACK);

	std::string mouseXString = "X: ";
	std::string mouseYString = "Y: ";
	DrawString(posX - 10, ScreenHeight() - SudokuUnitHeight,
		mouseXString.append(std::to_string(_mouseX)), olc::BLACK);
	DrawString(posX - 10, ScreenHeight() - SudokuUnitHeight / 2,
		mouseYString.append(std::to_string(_mouseY)), olc::BLACK);

}

void SudokuSolver::DrawBoard(void)
{
	if (_state == State::WaitingForInput)
	{
		FillRect(_activeCellX * SudokuUnitWidth, _activeCellY  * SudokuUnitHeight + TopLayerHeight, SudokuUnitWidth,
					SudokuUnitHeight, olc::GREY);
	}
	for (uint8_t indexX = 0; indexX < SudokuUnits; indexX++)
	{
		for (uint8_t indexY = 0; indexY < SudokuUnits; indexY++)
		{
			if (_board[indexX][indexY].value == 0)
			{
				continue;
			}
			if (_board[indexX][indexY].isFixed)
			{
				DrawString(indexX * SudokuUnitWidth + 12, indexY * SudokuUnitHeight + TopLayerHeight + 12,
					std::to_string(_board[indexX][indexY].value), olc::RED, 3);
			}
			else
			{
				DrawString(indexX * SudokuUnitWidth + 12, indexY * SudokuUnitHeight + TopLayerHeight + 12,
					std::to_string(_board[indexX][indexY].value), olc::BLACK, 3);
			}
		}
	}
}

void SudokuSolver::DrawHelpText(void)
{
	int32_t posX = 440;
	int32_t posY = 200;

	switch (_state)
	{
	case State::Idle:
		DrawString(posX, posY, "Waiting for input...", olc::BLACK);
		break;
	case State::WaitingForInput:
		if (_inputValid)
		{
			DrawString(posX, posY, "Type a number...", olc::BLACK);
			break;
		}
		DrawString(posX, posY, "Invalid input!", olc::BLACK);
		break;
	case State::Running:
		DrawString(posX, posY, "Running...", olc::BLACK);
		break;
	case State::Pause:
		DrawString(posX, posY, "Pausing...", olc::BLACK);
		break;
	case State::Finished:
		DrawString(posX, posY, "Finished!", olc::BLACK);
		break;
	default:
		break;
	}

	if (_state == State::Running || _state == State::Pause || _state == State::Finished)
	{
		std::string iterationString = "Iterations: ";
		iterationString.append(std::to_string(_iterations));
		DrawString(posX, posY + 100, iterationString, olc::BLACK);
	}

}