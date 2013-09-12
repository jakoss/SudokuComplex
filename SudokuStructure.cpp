#include "SudokuStructure.h"

SudokuStructure::SudokuStructure(SudokuPuzzle sudoku, SudokuPuzzle solvedSudoku, SudokuPuzzle originalSudoku)
{
    this->sudoku = sudoku;
    this->solvedSudoku = solvedSudoku;
    this->originalSudoku = originalSudoku;
}

SudokuPuzzle SudokuStructure::getSudoku()
{
    return this->sudoku;
}

SudokuPuzzle SudokuStructure::getSolvedSudoku()
{
    return this->solvedSudoku;
}

SudokuPuzzle SudokuStructure::getOriginalSudoku()
{
    return this->originalSudoku;
}


