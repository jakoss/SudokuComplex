#ifndef SUDOKUSTRUCTURE_H
#define SUDOKUSTRUCTURE_H

#include "Types.h"

class SudokuStructure{
private:
    SudokuPuzzle sudoku;
    SudokuPuzzle solvedSudoku;
    SudokuPuzzle originalSudoku;
public:
    SudokuStructure(SudokuPuzzle sudoku, SudokuPuzzle solvedSudoku, SudokuPuzzle originalSudoku);

    SudokuPuzzle getSudoku();
    SudokuPuzzle getSolvedSudoku();
    SudokuPuzzle getOriginalSudoku();
};

#endif // SUDOKUSTRUCTURE_H
