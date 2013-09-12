#ifndef SUDOKUCLASS_H
#define SUDOKUCLASS_H

#include "Cell.h"
#include "Types.h"
#include <vector>
#include "SudokuStructure.h"
#include "sudokusettings.h"

class SudokuClass{
public:
    SudokuClass(SudokuStructure * sudoku, SudokuSettings * settings);
    Cell * getCell(int posX, int posY);
    void clickCell(int posX, int posY);
    void numberClicked(int number);
    void clearMemory();
    void updateCellVal(int posX, int posY);
    bool checkSudoku();
    SudokuStructure * getSudokuStructure();
    std::vector<int> updateNumberState();
    std::vector<int> getNumberState();
    Cell * getHint();

private:
    SudokuStructure * sudoku;
    SudokuSettings * settings;
    std::vector<int> numberState;
    void defaultColors();

};

#endif // SUDOKUCLASS_H
