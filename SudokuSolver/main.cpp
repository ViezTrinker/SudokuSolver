#include "config.h"
#include "sudoku_solver.h"

int main()
{
    SudokuSolver solver;
    if (solver.Construct(ScreenWidth, ScreenHeight, PixelWidth, PixelHeight))
    {
        solver.Start();
    }
}