#ifndef HELPERCLASS_H
#define HELPERCLASS_H

#include "Types.h"
#include "SudokuStructure.h"

class HelperClass{
public:
    static SudokuPuzzle getSudokuPuzzleFromTable(int sudoku[]);
    static void saveSudokuPuzzleToFile(SudokuStructure *ss);
    static SudokuStructure *loadSudokuFromFile();
};

#endif // HELPERCLASS_H
